#pragma section REPRO
#include "memory_dump.h"

#include <string.h> // for memcpy

#include "../System/TimeManager/time_manager.h"
#include "../CmdTlm/packet_handler.h"
#include "../Library/endian_memcpy.h"

static MemoryDump memory_dump_;
const MemoryDump* const memory_dump = &memory_dump_;

static CTCP MEM_ctcp_; // データサイズが大きいのでstatic確保

static void MEM_init_(void);

static uint8_t MEM_get_next_adu_counter_(void);

static CCP_EXEC_STS MEM_dump_region_(uint8_t category,
                                     uint8_t num_dumps);

static MEM_ACK MEM_form_packet_(TCP* packet,
                                uint8_t category);

static TCP_SEQ_FLAG MEM_judge_seq_flag_(uint32_t rp,
                                        size_t len);

static void MEM_setup_header_(TCP* packet,
                              size_t data_len,
                              uint8_t category,
                              uint8_t adu_cnt,
                              TCP_SEQ_FLAG adu_seq_flag,
                              uint16_t adu_seq_cnt,
                              uint32_t adu_len);

static void MEM_send_packet_(const CTCP* packet,
                             uint8_t num_dumps);

AppInfo MEM_create_app(void)
{
  return create_app_info("mem", MEM_init_, NULL);
}

static void MEM_init_(void)
{
  memory_dump_.begin = 0;
  memory_dump_.end = 0;
  memory_dump_.adu_size = 0;
  memory_dump_.adu_seq = 0;
  memory_dump_.adu_counter = 0;
}

CCP_EXEC_STS Cmd_MEM_SET_REGION(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t begin, end, span;

  // パラメータを読み出し
  endian_memcpy(&begin, param, 4);
  endian_memcpy(&end,   param + 4, 4);

  if (begin > end)
  {
    // 領域の開始と終了の大小関係が逆の場合は異常終了
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  span = end - begin;

  if (span > MEM_MAX_SPAN)
  {
    // 指定ダンプ幅が最大量を超えている場合は異常終了。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  memory_dump_.begin = begin;
  memory_dump_.end = end;
  memory_dump_.adu_size = span;
  memory_dump_.adu_seq = 0;

  // 領域設定1回毎に独立したADUカウント値を割り当てる。
  memory_dump_.adu_counter = MEM_get_next_adu_counter_();

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_MEM_DUMP_REGION_SEQ(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t category, num_dumps;

  // パラメータを読み出し
  category = param[0];
  num_dumps = param[1];

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  return MEM_dump_region_(category, num_dumps);
}

CCP_EXEC_STS Cmd_MEM_DUMP_REGION_RND(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t category, num_dumps;
  uint16_t adu_seq;
  uint32_t rp;

  // パラメータを読み出し
  category = param[0];
  num_dumps = param[1];

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  endian_memcpy(&adu_seq, param + 2, 2);

  rp = memory_dump_.begin + (adu_seq * MEM_DUMP_WIDTH);

  if (rp < memory_dump_.end)
  {
    // 指定されたADU Sequence Counterが領域内なら値を取り込み、
    // 指定された位置の内容をダンプ。
    // 「指定する = ダンプしたい」と判断している
    memory_dump_.adu_seq = adu_seq;
    return MEM_dump_region_(category, num_dumps);
  }
  else
  {
    // 指定されたADU Sequence Counterが領域外であれば異常終了
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }
}

CCP_EXEC_STS Cmd_MEM_DUMP_SINGLE(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t category, num_dumps;
  uint32_t start_addr;
  uint8_t* data;

  // パラメータを読み出し
  category = param[0];
  num_dumps = param[1];

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  endian_memcpy(&start_addr, param + 2, 4);

  // 要検討: 指定アドレス値が異常な場合の処理をすべきか？
  // Segmentation Faultとか起こる？

  // メモリダンプパケットのヘッダを設定
  // Packet Sequence Counter for each ADUは単パケットなので0固定。
  // ADU分割形式にしなければ5Bytes分ダンプ領域を増やせるが、
  // Packet IDも別にする必要が生じるのでひとまず形式を合わせて実装する。
  MEM_setup_header_(&MEM_ctcp_,
                    MEM_DUMP_WIDTH,              // 1パケットの最大長をダンプする
                    category,                    // 送出カテゴリはパラメータで指定
                    MEM_get_next_adu_counter_(), // 単ADUなのでADUカウント値を直接割り当て
                    TCP_SEQ_SINGLE,              // アドレス指定の場合は単ADUとして処理
                    0x0000,                      // 単ADUなので0x0000に固定
                    MEM_DUMP_WIDTH);             // ADU長はDUMP長と同一

  // ダンプデータを設定
  data = TCP_TLM_get_user_data_head(&MEM_ctcp_) + 5; // ADU分割によりヘッダ長が5Bytes増える
  memcpy(data, (const void*)start_addr, MEM_DUMP_WIDTH);

  // 生成したパケットを送出
  MEM_send_packet_(&MEM_ctcp_, num_dumps);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_MEM_LOAD(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t param_len = CCP_get_param_len(packet);
  size_t data_len;
  uint32_t start_addr;

  // データ長読み出し
  // データ長 -> パラメータ全長-開始アドレスパラメータ長
  data_len = param_len - 4;

  // 書き込みアドレス読み出し
  endian_memcpy(&start_addr, param, 4);

  // 指定した開始アドレスから始まる領域にデータを書き込み
  memcpy((void*)start_addr, &(param[4]), data_len);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_MEM_SET_DESTINATION(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t dest;

  endian_memcpy(&dest, param, 4);

  if ((dest >= memory_dump_.begin) && (dest < memory_dump_.end))
  {
    // 宛先アドレスが領域内部に含まれる場合。
    // これを認めると処理が複雑になるので禁止する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 宛先アドレスを設定し、RPを領域先頭に合わせる。
  memory_dump_.dest = dest;
  memory_dump_.rp = memory_dump_.begin;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_MEM_COPY_REGION_SEQ(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t copy_width, wp;

  if (memory_dump_.rp == memory_dump_.end)
  {
    // 既に領域全体の読み出しが完了している場合。
    // 処理は行わず正常終了する。
    return CCP_EXEC_SUCCESS;
  }

  // パラメータ読み出し。
  endian_memcpy(&copy_width, param, 4);

  if ((memory_dump_.rp + copy_width) > memory_dump_.end)
  {
    // 指定コピー幅でコピーすると領域外に出てしまう場合。
    // コピー結果が領域終端に一致するようコピー幅をクリップ。
    copy_width = memory_dump_.end - memory_dump_.rp;
  }

  // WP計算。WP = 宛先先頭+読み出し済みデータ量。
  wp = memory_dump_.dest + (memory_dump_.rp - memory_dump_.begin);

  // 指定されたコピー幅だけ領域をコピーし、RPを更新。
  memcpy((uint8_t*)wp, (const uint8_t*)memory_dump_.rp, copy_width);
  memory_dump_.rp += copy_width;
  return CCP_EXEC_SUCCESS;
}

static uint8_t MEM_get_next_adu_counter_(void)
{
  // インクリメント後の値を返すので初期値は0xffとする。
  static uint8_t adu_counter_ = 0xff;
  return ++adu_counter_;
}

static CCP_EXEC_STS MEM_dump_region_(uint8_t category,
                                     uint8_t num_dumps)
{
  MEM_ACK ack;

  // 設定値にもとづき送出すべきパケットを構築
  ack = MEM_form_packet_(&MEM_ctcp_, category);

  switch (ack)
  {
  case MEM_SUCCESS:
    // 生成したパケットを送出し、ADU Sequence Counterの値を更新
    MEM_send_packet_(&MEM_ctcp_, num_dumps);
    ++memory_dump_.adu_seq;
    return CCP_EXEC_SUCCESS;

  case MEM_NO_DATA:
    // すでに全領域ダンプ済みなら何もせず終了
    return CCP_EXEC_SUCCESS;

  default:
    // それ以外のエラーはないはず
    return CCP_EXEC_UNKNOWN;
  }
}

static MEM_ACK MEM_form_packet_(TCP* packet,
                                uint8_t category)
{
  uint32_t rp;
  size_t len;
  TCP_SEQ_FLAG seq_flag;
  uint8_t* data;

  rp = memory_dump_.begin + (memory_dump_.adu_seq * MEM_DUMP_WIDTH);

  if (rp >= memory_dump_.end)
  {
    // Read Pointerが終端に達していれば何もせず終了
    return MEM_NO_DATA;
  }

  // 残りダンプ長を計算
  len = memory_dump_.end - rp;

  if (len > MEM_DUMP_WIDTH)
  {
    // 1パケットのダンプ幅を超える場合はダンプ幅に押さえる。
    len = MEM_DUMP_WIDTH;
  }

  // Packet Sequence Flagの種別判定
  seq_flag = MEM_judge_seq_flag_(rp, len);

  // メモリダンプパケットのヘッダを設定
  MEM_setup_header_(&MEM_ctcp_,
                    len,
                    category,
                    memory_dump_.adu_counter,
                    seq_flag,
                    memory_dump_.adu_seq,
                    memory_dump_.adu_size);

  // ダンプデータを設定
  data = TCP_TLM_get_user_data_head(packet) + 5; // ADU分割によりヘッダ長が5Bytes増える
  memcpy(data, (const void*)rp, len);

  return MEM_SUCCESS;
}

static TCP_SEQ_FLAG MEM_judge_seq_flag_(uint32_t rp,
                                        size_t len)
{
  if (memory_dump_.adu_size < MEM_DUMP_WIDTH)
  {
    // ADUが単パケットに収まる場合
    return TCP_SEQ_SINGLE;
  }
  else
  {
    // ADUが複数パケットにまたがる場合
    if (rp == memory_dump_.begin)
    {
      // Read Pointerが読み出し領域先頭に一致するなら開始パケット
      return TCP_SEQ_FIRST;
    }
    else if (len < MEM_DUMP_WIDTH)
    {
      // ダンプ残量が1パケット以内に収まるなら終了パケット
      return TCP_SEQ_LAST;
    }
    else
    {
      // それ以外の場合は中間パケット
      return TCP_SEQ_CONT;
    }
  }
}

static void MEM_setup_header_(TCP* packet,
                              size_t data_len,
                              uint8_t category,
                              uint8_t adu_cnt,
                              TCP_SEQ_FLAG adu_seq_flag,
                              uint16_t adu_seq_cnt,
                              uint32_t adu_len)
{
  uint8_t* data;

  // TCPacketヘッダを設定
  TCP_TLM_setup_primary_hdr(packet, TCP_APID_DUMP_TLM, (uint16_t)(data_len + 12));
  TCP_TLM_set_ti(packet, (uint32_t)( TMGR_get_master_total_cycle() ));
  TCP_TLM_set_category(packet, category); // パラメータによる指定
  TCP_TLM_set_packet_id(packet, (uint8_t)MEM_TLM_ID);
  TCP_TLM_set_adu_cnt(packet, adu_cnt);
  TCP_TLM_set_adu_seq_flag(packet, adu_seq_flag);

  // ADU分割時のSecondary Header追加分を設定
  data = TCP_TLM_get_user_data_head(packet);
  data[0] = (uint8_t)(adu_seq_cnt >> 8);
  data[1] = (uint8_t)(adu_seq_cnt);
  data += 2;
  // ADU Lengthを設定 3Bytes長なので注意
  data[0] = (uint8_t)(adu_len >> 16);
  data[1] = (uint8_t)(adu_len >> 8);
  data[2] = (uint8_t)(adu_len);
}

static void MEM_send_packet_(const CTCP* packet,
                             uint8_t num_dumps)
{
  int i;

  // TLM送出処理
  for (i = 0; i < num_dumps; ++i)
  {
    // num_dumpsの回数だけ生成したパケットを配送処理へ渡す
    PH_analyze_packet(packet);
  }
}

#pragma section
