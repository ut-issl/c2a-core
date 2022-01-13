#pragma section REPRO
#include "telemetry_generator.h"

#include <stddef.h> // for NULL

#include "telemetry_frame.h"
#include "packet_handler.h"
#include "../System/TimeManager/time_manager.h"
#include <src_user/CmdTlm/telemetry_definitions.h>

static uint8_t TG_get_next_adu_counter_(void);

CCP_EXEC_STS Cmd_GENERATE_TLM(const CTCP* packet)
{
  static TCP tcp_;
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t category;
  uint8_t id;
  uint8_t num_dumps;
  int len;

  // パラメータ読み出し
  category = param[0];
  id = param[1];
  num_dumps = param[2];

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // ADU生成
  // ADU分割が発生しない場合に限定したコードになっている。
  // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
  len = TF_generate_contents((int)id,
                             TCP_TLM_get_user_data_head(&tcp_),
                             TCP_MAX_LEN - TCP_PRM_HDR_LEN - TCP_TLM_2ND_HDR_LEN);

  // 範囲外のTLM IDを除外
  if (len == TF_NOT_DEFINED) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (len < 0) return CCP_EXEC_ILLEGAL_CONTEXT;     // TODO: lenがマイナスの値たちをどうするか？

  // TCPacketヘッダ設定
  TCP_TLM_setup_primary_hdr(&tcp_, TCP_APID_MIS_TLM, (uint16_t)(len + 7));
  TCP_TLM_set_ti(&tcp_, (uint32_t)(TMGR_get_master_total_cycle()));
  TCP_TLM_set_category(&tcp_, category); // パラメータによる指定
  TCP_TLM_set_packet_id(&tcp_, id);
  TCP_TLM_set_adu_seq_flag(&tcp_, TCP_SEQ_SINGLE);
  TCP_TLM_set_adu_cnt(&tcp_, TG_get_next_adu_counter_());

  // 生成したパケットを指定された回数配送処理へ渡す
  while (num_dumps != 0)
  {
    PH_analyze_packet(&tcp_);
    --num_dumps;
  }

  return CCP_EXEC_SUCCESS;
}

static uint8_t TG_get_next_adu_counter_(void)
{
  // インクリメントした値を返すため初期値は0xffとする
  static uint8_t adu_counter_ = 0xff;
  return ++adu_counter_;
}

#pragma section
