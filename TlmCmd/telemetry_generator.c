#pragma section REPRO
#include "telemetry_generator.h"

#include <stddef.h> // for NULL

#include "telemetry_frame.h"
#include "packet_handler.h"
#include "../System/TimeManager/time_manager.h"
#include "common_cmd_packet.h"
#include "common_cmd_packet_util.h"
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/Settings/TlmCmd/common_tlm_packet_define.h>
#include "./Ccsds/tlm_space_packet.h"   // FIXME: TSP 依存性はNGなので， TCP → SP 大工事終わったら直す

static uint8_t TG_get_next_adu_counter_(void);


// FIXME: 現在のコードは，MOBC と 2nd OBC の Tlm id がユニークであることを想定している
//        本来被っても良いはず
CCP_CmdRet Cmd_GENERATE_TLM(const CommonCmdPacket* packet)
{
  static CommonTlmPacket ctp_;
  uint8_t category = CCP_get_param_from_packet(packet, 0, uint8_t);
  TLM_CODE id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);
  uint8_t num_dumps = CCP_get_param_from_packet(packet, 2, uint8_t);
  uint16_t len;
  TF_TLM_FUNC_ACK ack;
  uint8_t dr_partition_mask = 0x1f; // 00011111b        // FIXME: 一時的な対応
  uint8_t dest_flags_mask = 0xe0; // 11100000b        // FIXME: 一時的な対応
  uint8_t dr_partition;
  ctp_dest_flags_t dest_flags;

  if (num_dumps >= 8)
  {
    // パケット生成回数の上限は8回とする。
    // 32kbpsでのDL時に8VCDU/secで1秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // ctp の ヘッダ部分の APID をクリア
  // この後で， APID_is_other_obc_tlm_apid で配送元 OBC を割り出せるように
  CTP_set_apid(&ctp_, APID_UNKNOWN);

  // ADU生成
  // ADU分割が発生しない場合に限定したコードになっている。
  // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
  ack = TF_generate_contents(id,
                             ctp_.packet,
                             &len,
                             TSP_MAX_LEN);

  // 範囲外のTLM IDを除外
  if (ack == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (ack != TF_TLM_FUNC_ACK_SUCCESS) return CCP_EXEC_ILLEGAL_CONTEXT;

  // Header
  if (APID_is_other_obc_tlm_apid(CTP_get_apid(&ctp_)))
  {
    // 2nd OBC で生成された TLM の primary header, secondary header の board time はそのまま維持
  }
  else
  {
    // Primary Header
    // FIXME: Space Packet 依存を直す
    TSP_setup_primary_hdr(&ctp_, CTP_APID_FROM_ME, len);
    TSP_set_seq_count(&ctp_, TG_get_next_adu_counter_());

    // Secondary Header
    TSP_set_board_time(&ctp_, (uint32_t)(TMGR_get_master_total_cycle()));
  }

  // FIXME: 他の時刻も入れる
  CTP_set_global_time(&ctp_);
  TSP_set_on_board_subnet_time(&ctp_, (uint32_t)(TMGR_get_master_total_cycle()));   // FIXME: 暫定

  // FIXME: 他 OBC からのパケットは別処理する
  // FIXME: 一旦雑に category を処理してるが後でちゃんと直す
  dr_partition = (uint8_t)(category & dr_partition_mask);
  dest_flags = (uint8_t)( (category & dest_flags_mask) >> 5 );
  if (dest_flags == 0)
  {
    dest_flags = CTP_DEST_FLAG_RP;
  }
  TSP_set_dest_flags(&ctp_, dest_flags);
  TSP_set_dr_partition(&ctp_, dr_partition);
  TSP_set_tlm_id(&ctp_, id);

  // 生成したパケットを指定された回数配送処理へ渡す
  while (num_dumps != 0)
  {
    PH_analyze_tlm_packet(&ctp_);
    --num_dumps;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// FIXME: space packet 大工事でビット幅が変わってるので直す！
static uint8_t TG_get_next_adu_counter_(void)
{
  // インクリメントした値を返すため初期値は0xffとする
  static uint8_t adu_counter_ = 0xff;
  return ++adu_counter_;
}

#pragma section
