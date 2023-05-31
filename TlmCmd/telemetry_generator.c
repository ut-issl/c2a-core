#pragma section REPRO
#include "telemetry_generator.h"

#include <stddef.h> // for NULL

#include "telemetry_frame.h"
#include "packet_handler.h"
#include "../System/TimeManager/time_manager.h"
#include "common_cmd_packet.h"
#include "common_cmd_packet_util.h"
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/TlmCmd/user_packet_handler.h>
#include <src_user/Settings/TlmCmd/common_tlm_packet_define.h>
#include "./Ccsds/tlm_space_packet.h"   // FIXME: TSP 依存性はNGなので， TCP → SP 大工事終わったら直す


/**
 * @brief 自身のテレメを生成
 * @note  2nd OBC のテレメは生成できない
 * @param[in] tlm_id:     Tlm ID
 * @param[in] dest_flags: Dest Flags
 * @param[in] dest_info:  Dest Info
 * @param[in] dump_num:   テレメ送出回数
 * @return CCP_CmdRet
 */
static CCP_CmdRet TG_generate_tlm_(TLM_CODE tlm_id,
                                   ctp_dest_flags_t dest_flags,
                                   uint8_t dest_info,
                                   uint8_t num_dumps);

/**
 * @brief 2nd OBC のテレメを転送
 * @note  自身の OBC のテレメは転送できない
 * @param[in] apid:       転送する 2nd OBC tlm の APID
 * @param[in] tlm_id:     Tlm ID
 * @param[in] dest_flags: Dest Flags
 * @param[in] dest_info:  Dest Info
 * @param[in] dump_num:   テレメ送出回数
 * @return CCP_CmdRet
 */
static CCP_CmdRet TG_forward_tlm_(APID apid,
                                  TLM_CODE tlm_id,
                                  ctp_dest_flags_t dest_flags,
                                  uint8_t dest_info,
                                  uint8_t dump_num);

/**
 * @brief 次のパケットで用いる Sequence Count を取得
 * @param  void
 * @return Sequence Count
 */
static uint16_t TG_get_next_seq_count_(void);

static CommonTlmPacket TG_ctp_;


// FIXME: 現在のコードは，MOBC と 2nd OBC の Tlm id がユニークであることを想定している
//        本来被っても良いはず
// DEPRECATED!!!!!
CCP_CmdRet Cmd_GENERATE_TLM(const CommonCmdPacket* packet)
{
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
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // ctp の ヘッダ部分の APID をクリア
  // この後で， 配送元 OBC が自身か 2nd obc かを割り出せるように
  CTP_set_apid(&TG_ctp_, (APID)(APID_UNKNOWN & 0x7ff));    // FIXME: APID_UNKNOWN = APID_FILL_PKT + 1 だと 11 bit から溢れてる...

  // ADU生成
  // ADU分割が発生しない場合に限定したコードになっている。
  // TLM定義シート上で定義するADUはADU長をADU分割が発生しない長さに制限する。
  ack = TF_generate_contents(id,
                             TG_ctp_.packet,
                             &len,
                             TSP_MAX_LEN);

  // 範囲外のTLM IDを除外
  if (ack == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (ack != TF_TLM_FUNC_ACK_SUCCESS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  // Header
  if ((APID)(CTP_get_apid(&TG_ctp_) & 0x7ff) != (APID)(APID_UNKNOWN & 0x7ff))    // FIXME: APID_UNKNOWN = APID_FILL_PKT + 1 だと 11 bit から溢れてる...
  {
    // 2nd OBC で生成された TLM の primary header, secondary header の board time はそのまま維持
  }
  else
  {
    // Primary Header
    // FIXME: Space Packet 依存を直す
    TSP_setup_primary_hdr(&TG_ctp_, CTP_APID_FROM_ME, TG_get_next_seq_count_(), len);

    // Secondary Header
    TSP_set_board_time(&TG_ctp_, (uint32_t)(TMGR_get_master_total_cycle()));
  }

  // FIXME: 他の時刻も入れる
  CTP_set_global_time(&TG_ctp_);
  TSP_set_on_board_subnet_time(&TG_ctp_, (uint32_t)(TMGR_get_master_total_cycle()));   // FIXME: 暫定

  // FIXME: 他 OBC からのパケットは別処理する
  // FIXME: 一旦雑に category を処理してるが後でちゃんと直す
  dr_partition = (uint8_t)(category & dr_partition_mask);
  dest_flags = (uint8_t)( (category & dest_flags_mask) >> 5 );
  if (dest_flags == 0)
  {
    dest_flags = CTP_DEST_FLAG_RP_TLM;
  }
  TSP_set_dest_flags(&TG_ctp_, dest_flags);
  TSP_set_dest_info(&TG_ctp_, dr_partition);   // FIXME: もはや dr partition ですらない
  TSP_set_tlm_id(&TG_ctp_, id);
  TSP_set_2nd_hdr_ver(&TG_ctp_, TSP_2ND_HDR_VER_1);

  // 生成したパケットを指定された回数配送処理へ渡す
  while (num_dumps != 0)
  {
    PH_analyze_tlm_packet(&TG_ctp_);
    --num_dumps;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_TG_GENERATE_TLM(const CommonCmdPacket* packet)
{
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);
  ctp_dest_flags_t dest_flags = (ctp_dest_flags_t)CCP_get_param_from_packet(packet, 1, uint8_t);
  uint8_t dest_info = CCP_get_param_from_packet(packet, 2, uint8_t);
  uint8_t dump_num = CCP_get_param_from_packet(packet, 3, uint8_t);

  return TG_generate_tlm_(tlm_id, dest_flags, dest_info, dump_num);
}


CCP_CmdRet Cmd_TG_GENERATE_HK_TLM(const CommonCmdPacket* packet)
{
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);

  return TG_generate_tlm_(tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_HK_TLM, 0, 1);
}


CCP_CmdRet Cmd_TG_GENERATE_MS_TLM(const CommonCmdPacket* packet)
{
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);

  return TG_generate_tlm_(tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_MS_TLM, 0, 1);
}


CCP_CmdRet Cmd_TG_GENERATE_ST_TLM(const CommonCmdPacket* packet)
{
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 1, uint8_t);

  return TG_generate_tlm_(tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_ST_TLM, dr_partition, 1);
}


CCP_CmdRet Cmd_TG_FORWARD_TLM(const CommonCmdPacket* packet)
{
  APID apid = APID_get_apid_from_uint16(CCP_get_param_from_packet(packet, 0, uint16_t));
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);
  ctp_dest_flags_t dest_flags = (ctp_dest_flags_t)CCP_get_param_from_packet(packet, 2, uint8_t);
  uint8_t dest_info = CCP_get_param_from_packet(packet, 3, uint8_t);
  uint8_t dump_num = CCP_get_param_from_packet(packet, 4, uint8_t);

  return TG_forward_tlm_(apid, tlm_id, dest_flags, dest_info, dump_num);
}


CCP_CmdRet Cmd_TG_FORWARD_AS_HK_TLM(const CommonCmdPacket* packet)
{
  APID apid = APID_get_apid_from_uint16(CCP_get_param_from_packet(packet, 0, uint16_t));
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);

  return TG_forward_tlm_(apid, tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_HK_TLM, 0, 1);
}


CCP_CmdRet Cmd_TG_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet)
{
  APID apid = APID_get_apid_from_uint16(CCP_get_param_from_packet(packet, 0, uint16_t));
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);

  return TG_forward_tlm_(apid, tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_MS_TLM, 0, 1);
}


CCP_CmdRet Cmd_TG_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet)
{
  APID apid = APID_get_apid_from_uint16(CCP_get_param_from_packet(packet, 0, uint16_t));
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 1, uint8_t);
  uint8_t dr_partition = CCP_get_param_from_packet(packet, 2, uint8_t);

  return TG_forward_tlm_(apid, tlm_id, (ctp_dest_flags_t)CTP_DEST_FLAG_ST_TLM, dr_partition, 1);
}


static CCP_CmdRet TG_generate_tlm_(TLM_CODE tlm_id,
                                   ctp_dest_flags_t dest_flags,
                                   uint8_t dest_info,
                                   uint8_t dump_num)
{
  TF_TLM_FUNC_ACK ack;
  uint16_t packet_len;

  if (dump_num >= 8)
  {
    // FIXME: 要検討？
    // パケット生成回数の上限は 8 回とする。
    // 32 kbpsでの DL 時に 8 VCDU / sec で 1 秒分の通信量。
    // これを超える場合は複数回コマンドを送信して対応する。
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER, TLM_CODE_MAX);
  }

  // ADU 生成
  // ADU 分割が発生しない場合に限定したコードになっている
  // TLM 定義シート上で定義する ADU は ADU 長を ADU 分割が発生しない長さに制限する
  // FIXME: ↑ ADU，今の TSP では存在しない？ 文面見直してなおす
  //          https://github.com/ut-issl/c2a-core/issues/222
  ack = TF_generate_contents(tlm_id,
                             TG_ctp_.packet,
                             &packet_len,
                             TSP_MAX_LEN);

  // 範囲外のTLM IDを除外
  if (ack == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER, tlm_id);
  if (ack != TF_TLM_FUNC_ACK_SUCCESS) return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, (uint32_t)ack);

  // 自身の OBC のテレメ生成を前提としているので， Cmd_GENERATE_TLM のように 2nd OBC 判定はいれない

  // Primary Header
  TSP_setup_primary_hdr(&TG_ctp_, CTP_APID_FROM_ME, TG_get_next_seq_count_(), packet_len);

  // Secondary Header
  TSP_set_2nd_hdr_ver(&TG_ctp_, TSP_2ND_HDR_VER_1);
  TSP_set_board_time(&TG_ctp_, (uint32_t)(TMGR_get_master_total_cycle()));
  TSP_set_tlm_id(&TG_ctp_, tlm_id);
  // FIXME: 他の時刻も入れる
  CTP_set_global_time(&TG_ctp_);
  CTP_set_on_board_subnet_time(&TG_ctp_);
  TSP_set_dest_flags(&TG_ctp_, dest_flags);
  TSP_set_dest_info(&TG_ctp_, dest_info);

  // 生成したパケットを指定された回数配送処理へ渡す
  while (dump_num != 0)
  {
    PH_analyze_tlm_packet(&TG_ctp_);
    --dump_num;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


static CCP_CmdRet TG_forward_tlm_(APID apid,
                                  TLM_CODE tlm_id,
                                  ctp_dest_flags_t dest_flags,
                                  uint8_t dest_info,
                                  uint8_t dump_num)
{
  TF_TLM_FUNC_ACK ack;
  uint16_t packet_len;

  if (dump_num >= 8)
  {
    // FIXME: TG_generate_tlm_ とともに要検討
    return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER, 0);
  }

  ack = PH_user_telemetry_router(apid,
                                 tlm_id,
                                 TG_ctp_.packet,
                                 &packet_len,
                                 TSP_MAX_LEN);

  if (ack == TF_TLM_FUNC_ACK_NOT_DEFINED) return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_PARAMETER, ( ((uint16_t)apid << 16) | (0x0000ffff & tlm_id) ));
  if (ack != TF_TLM_FUNC_ACK_SUCCESS) return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, (uint32_t)ack);

  // 2nd OBC なので， Header は可能な限り維持
  // Primary Header → 維持

  // Secondary Header
  // FIXME: 方針を決めて直す（また， `(uint64_t)` へのキャストは不適切そう）
  // if ((uint64_t)TSP_get_global_time(&TG_ctp_) == 0xffffffffffffffffULL)
  // {
  //   CTP_set_global_time(&TG_ctp_);
  // }
  if (CTP_get_on_board_subnet_time(&TG_ctp_) == 0xffffffff)
  {
    // FIXME: 本当は Driver で受信時に上書きするべき？ 一応 CTP_get_ctp_from_dssc でも対応
    // MOBC - 2nd OBC - 3rd OBC というとき， 2nd OBC でも 0xffffffff ができるようにしている
    CTP_set_on_board_subnet_time(&TG_ctp_);
  }

  TSP_set_dest_flags(&TG_ctp_, dest_flags);
  TSP_set_dest_info(&TG_ctp_, dest_info);

  // 生成したパケットを指定された回数配送処理へ渡す
  while (dump_num != 0)
  {
    PH_analyze_tlm_packet(&TG_ctp_);
    --dump_num;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


static uint16_t TG_get_next_seq_count_(void)
{
  // インクリメントした値を返すため初期値は 0xffff とする
  static uint16_t adu_counter_ = 0xffff;
  ++adu_counter_;
  // Sequence Count は 14 bit
  return 0x3fff & adu_counter_;
}

#pragma section
