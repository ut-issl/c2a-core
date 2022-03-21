#pragma section REPRO
/**
 * @file
 * @brief 受信した TC Frame の検証を行う
 */

#include "gs_validate.h"
#include "../../TlmCmd/Ccsds/tc_segment.h"
#include <src_core/TlmCmd/Ccsds/space_packet_typedef.h>

#define GS_RECEIVE_WINDOW (256)
#define GS_POSITIVE_WINDOW_WIDTH_DEFAULT (64) // FIXME: 要検討
#if 2 * GS_POSITIVE_WINDOW_WIDTH_DEFAULT >= GS_RECEIVE_WINDOW
#error POSITIVE WINDOW SETTINGS IS WRONG
#endif

// 以下検証関数. 名前通り
static GS_VALIDATE_ERR GS_check_tctf_header_(const TcTransferFrame* tctf);

static GS_VALIDATE_ERR GS_check_tc_segment_(const TcSegment* tc_segment);
static GS_VALIDATE_ERR GS_check_tcs_headers_(const TcSegment* tc_segment);
static GS_VALIDATE_ERR GS_check_cmd_space_packet_headers_(const CmdSpacePacket* csp);

/**
 * @note AD コマンド: COP-1 制御を使用し伝送順番のチェックを行う
 */
static GS_VALIDATE_ERR GS_check_ad_cmd_(const TcTransferFrame* tctf);

/**
 * @note BD コマンド: COP-1 制御を使用しないため伝送順番のチェックは行わない
 */
static GS_VALIDATE_ERR GS_check_bd_cmd_(const TcTransferFrame* tctf);

/**
 * @note BC コマンド: COP-1 制御の制御コマンド, AD, BD と性質が異なる
 */
static GS_VALIDATE_ERR GS_check_bc_cmd_(const TcTransferFrame* tctf);

static GS_ValiateInfo gs_validate_info_;
const GS_ValiateInfo* const gs_validate_info = &gs_validate_info_;

void GS_validate_init(void)
{
  gs_validate_info_.lockout_flag = 0;
  gs_validate_info_.type_a_counter = 0;
  gs_validate_info_.type_b_counter = 0;
  gs_validate_info_.retransmit_flag = 0;
  gs_validate_info_.positive_window_width = GS_POSITIVE_WINDOW_WIDTH_DEFAULT;
}

GS_VALIDATE_ERR GS_validate_tctf(const TcTransferFrame* tctf)
{
  GS_VALIDATE_ERR ret;
  uint8_t is_frame_error;
  TCTF_TYPE tctf_type;

  ret = GS_check_tctf_header_(tctf);
  if (ret != GS_VALIDATE_ERR_OK) return ret;
  is_frame_error = TCTF_check_fecw(tctf);
  if (!is_frame_error) return GS_VALIDATE_ERR_FECW_MISSMATCH;

  tctf_type = TCTF_get_type(tctf);
  switch (tctf_type)
  {
  case TCTF_TYPE_AD:
    ret = GS_check_ad_cmd_(tctf);
    break;

  case TCTF_TYPE_BD:
    ret = GS_check_bd_cmd_(tctf);
    break;

  case TCTF_TYPE_BC:
    ret = GS_check_bc_cmd_(tctf);
    break;

  default:
    return GS_VALIDATE_ERR_TCTF_TYPE;
  }

  return ret;
}

static GS_VALIDATE_ERR GS_check_tctf_header_(const TcTransferFrame* tctf)
{
  if (TCTF_get_ver(tctf) != TCTF_VER_1) return GS_VALIDATE_ERR_TCTF_VER;
  if (TCTF_get_scid(tctf) != TCTF_SCID_SAMPLE_SATELLITE) return GS_VALIDATE_ERR_TCTF_SCID;
  if (TCTF_get_vcid(tctf) != TCTF_VCID_REALTIME) return GS_VALIDATE_ERR_TCTF_VCID;

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_tc_segment_(const TcSegment* tc_segment)
{
  GS_VALIDATE_ERR ack;
  const CmdSpacePacket* csp = TCS_get_command_space_packet(tc_segment);

  // TcSegment Header の固定値部分が妥当か確認する
  ack = GS_check_tcs_headers_(tc_segment);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  // CmdSpacePacket のヘッダのうち共通部分が妥当か確認する
  ack = GS_check_cmd_space_packet_headers_(csp);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_tcs_headers_(const TcSegment* tc_segment)
{
  if (TCS_get_seq_flag(tc_segment) != TCS_SEQ_SINGLE)
  {
    return GS_VALIDATE_ERR_TCS_SEQ_FLAG;
  }

  // FIXME: 以下要修正? (20210714)
  // ここではNORMAL以外のMAP_IDをサポートしない (000010b)
  // GSTOS側が送信するSegmentのMAP_IDは0x08となっている
  // 要修正:対策が完了するまで暫定的に判定処理を無効化

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_cmd_space_packet_headers_(const CmdSpacePacket* csp)
{
  APID apid;
  // FIXME: 他の部分のチェックも入れる

  if (CSP_get_ver(csp) != SP_VER_1) return GS_VALIDATE_ERR_CSP_VER;
  if (CSP_get_2nd_hdr_flag(csp) != SP_2ND_HDR_FLAG_PRESENT)
  {
    // ここではSecondary Headerが必須。
    return GS_VALIDATE_ERR_CSP_2ND_HDR_FLAG;
  }

  if (CSP_get_type(csp) != SP_TYPE_CMD)
  {
    return GS_VALIDATE_ERR_CSP_TYPE_IS_NOT_CMD;
  }

  apid = CSP_get_apid(csp);
  if ( !( apid == APID_MOBC_CMD ||
          apid == APID_AOBC_CMD ||
          apid == APID_TOBC_CMD ) )
  {
    return GS_VALIDATE_ERR_APID;
  }

  // Sequence Flag が単パケットか確認する
  if (CSP_get_seq_flag(csp) != SP_SEQ_FLAG_SINGLE)
  {
    return GS_VALIDATE_ERR_CSP_SEQ_IS_NOT_SINGLE;
  }

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_ad_cmd_(const TcTransferFrame* tctf)
{
  GS_VALIDATE_ERR ack;
  const TcSegment* tc_segment = TCTF_get_tc_segment(tctf);
  uint8_t seq_diff = (GS_RECEIVE_WINDOW + TCTF_get_frame_seq_num(tctf) - gs_validate_info_.type_a_counter) % GS_RECEIVE_WINDOW;

  if (gs_validate_info_.lockout_flag) return GS_VALIDATE_ERR_IN_LOCKOUT;

  ack = GS_check_tc_segment_(tc_segment);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  // N(R) == V(R)なら正常受信
  if (seq_diff == 0)
  {
    gs_validate_info_.retransmit_flag = 0;
    ++gs_validate_info_.type_a_counter;
    return GS_VALIDATE_ERR_OK;
  }
  // 送信側が行き過ぎているのでパケット破棄して再送要求
  else if (seq_diff <= gs_validate_info_.positive_window_width)
  {
    gs_validate_info_.retransmit_flag = 1;
    return GS_VALIDATE_ERR_FARM1_POSITIVE_WINDOW_AREA;
  }
  // もう既に送られているので破棄
  else if ((256 - gs_validate_info_.positive_window_width) <= seq_diff)
  {
    return GS_VALIDATE_ERR_FARM1_NEGATIVE_WINDOW_AREA;
  }
  // 許容 window から外れているので破棄してロックアウト, BC コマンドでロックアウト解除要求を行う
  // ロックアウトするので一切のコマンドを受け付けない
  else
  {
    gs_validate_info_.lockout_flag = 1;
    return GS_VALIDATE_ERR_FARM1_LOCKOUT_AREA;
  }

  // not reaced
  return GS_VALIDATE_ERR_UNKNOWN;
}

static GS_VALIDATE_ERR GS_check_bd_cmd_(const TcTransferFrame* tctf)
{
  GS_VALIDATE_ERR ack;
  const TcSegment* tc_segment = TCTF_get_tc_segment(tctf);

  ack = GS_check_tc_segment_(tc_segment);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  ++gs_validate_info_.type_b_counter;

  return GS_VALIDATE_ERR_OK;
}

// BCコマンドの種別を判定し、処理する
static GS_VALIDATE_ERR GS_check_bc_cmd_(const TcTransferFrame* tctf)
{
  uint16_t length = TCTF_get_frame_len(tctf);
  uint8_t offset = TCTF_HEADER_SIZE + TCTF_FECF_SIZE;
  const TcSegment* tc_segment = TCTF_get_tc_segment(tctf);

  // BC コマンドは COP-1 制御の制御用コマンドで特殊なため少し構造が異なる
  // Unlock
  if (tc_segment->packet[0] == TCTF_BC_CMD_CODE_UNLOCK && length == offset + 1)
  {
    gs_validate_info_.lockout_flag = 0;
    gs_validate_info_.retransmit_flag = 0;

    ++gs_validate_info_.type_b_counter;
  }
  // SET V(R)
  else if (tc_segment->packet[0] == TCTF_BC_CMD_CODE_SET_VR_1ST_BYTE &&
           tc_segment->packet[1] == TCTF_BC_CMD_CODE_SET_VR_2ND_BYTE &&
           length == offset + 3)
  {
    if (gs_validate_info_.lockout_flag == 0)
    {
      gs_validate_info_.type_a_counter = tc_segment->packet[2];
      gs_validate_info_.retransmit_flag = 0;
    }

    // Lockout状態でもType-B Counterの値は更新する
    ++gs_validate_info_.type_b_counter;
  }
  else
  {
    return GS_VALIDATE_ERR_INVALID_BC_CMD;
  }

  return GS_VALIDATE_ERR_OK;
}

uint32_t GS_form_clcw(void)
{
  // 値が0固定の項目は変数の初期化で代替し明示的には指定しない
  uint32_t clcw = 0;
  uint32_t val;

  // FIXME: Status Field
  // ***x xx** **** **** **** **** **** ****

  // COP in Effect -> COP-1
  clcw |= 0x01000000; // **** **01 **** **** **** **** **** ****

  // FIXME: No RF Available Flag
  // **** **** **** **** x*** **** **** ****
  // FIXME: NO Bit Lock Flag
  // **** **** **** **** *x** **** **** ****

  // Lockout flag
  if (gs_validate_info_.lockout_flag)
  {
    clcw |= 0x00002000; // **** **** **** **** **1* **** **** ****
  }
  // Retransmit
  if (gs_validate_info_.retransmit_flag)
  {
    clcw |= 0x00000800; // **** **** **** **** **** 1*** **** ****
  }

  // FARM B Counter
  val = (gs_validate_info_.type_b_counter & 0x03) << 9;
  clcw |= val; // **** **** **** **** **** *xx* **** ****

  // Type A Counter as Report Value
  clcw |= gs_validate_info_.type_a_counter; // **** **** **** **** **** **** xxxx xxxx

  return clcw;
}

GS_VALIDATE_ERR GS_set_farm_pw(uint8_t positive_window_width)
{
  if (positive_window_width < 1 || positive_window_width > 127)
  {
    return GS_VALIDATE_ERR_FARM1_POSITIVE_WINDOW_AREA;
  }

  gs_validate_info_.positive_window_width = positive_window_width;

  return GS_VALIDATE_ERR_OK;
}

#pragma section
