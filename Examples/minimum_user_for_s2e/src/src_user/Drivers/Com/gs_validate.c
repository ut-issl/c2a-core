#pragma section REPRO
/**
 * @file
 * @brief 受信した TC Frame の検証を行う
 */

#include "gs_validate.h"

#define GS_POSITIVE_WINDOW_WIDTH_DEFAULT (64) // FIXME: 要検討

// 以下検証関数. 名前通り
static GS_VALIDATE_ERR GS_check_tcf_header_(const TCF* tc_frame);
static GS_VALIDATE_ERR GS_check_tcf_contents_(const TCF* tc_frame);
static GS_VALIDATE_ERR GS_check_tcs_headers_(const TCS* tc_segment);
static GS_VALIDATE_ERR GS_check_cmd_space_packet_headers_(const CmdSpacePacket* csp);
static GS_VALIDATE_ERR GS_check_fecw_(const uint8_t* data, size_t len);

static GS_VALIDATE_ERR GS_check_ad_cmd_(const TCF* tc_frame);
static GS_VALIDATE_ERR GS_check_bc_cmd_(const TCF* tc_frame);
static GS_VALIDATE_ERR GS_check_bd_cmd_(const TCF* tc_frame);

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

GS_VALIDATE_ERR GS_validate_tc_frame(const TCF* tc_frame)
{
  GS_VALIDATE_ERR ret;

  size_t frame_length = TCF_get_frame_len(tc_frame);

  // TODO WINGS側がまだ対応してないので一旦コメントアウト
  GS_check_fecw_((const uint8_t*)tc_frame, frame_length);

  ret = GS_check_tcf_header_(tc_frame);
  if (ret != GS_VALIDATE_ERR_OK) return ret;

  switch (TCF_get_type(tc_frame))
  {
  case TCF_TYPE_AD:
    ret = GS_check_ad_cmd_(tc_frame);
    break;

  case TCF_TYPE_BC:
    ret = GS_check_bc_cmd_(tc_frame);
    break;

  case TCF_TYPE_BD:
    ret = GS_check_bd_cmd_(tc_frame);
    break;

  default:
    return GS_VALIDATE_ERR_TCF_TYPE;
  }

  return ret;
}

static GS_VALIDATE_ERR GS_check_tcf_header_(const TCF* tc_frame)
{
  if (TCF_get_ver(tc_frame) != TCF_VER_1) return GS_VALIDATE_ERR_TCF_VER;
  // if (TCF_get_scid(tc_frame) != TCF_SCID_SAMPLE_SATELLITE) return GS_VALIDATE_ERR_TCF_SCID;    // FIXME: テスト用に一旦コメントアウト
  if (TCF_get_vcid(tc_frame) != TCF_VCID_REALTIME) return GS_VALIDATE_ERR_TCF_VCID;

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_tcf_contents_(const TCF* tc_frame)
{
  GS_VALIDATE_ERR ack;

  // TCSegment Header の固定値部分が妥当か確認する
  ack = GS_check_tcs_headers_(&tc_frame->tcs);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  // CmdSpacePacket のヘッダのうち共通部分が妥当か確認する
  ack = GS_check_cmd_space_packet_headers_(&tc_frame->tcs.tcp);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_tcs_headers_(const TCS* tc_segment)
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

  if (CSP_get_ver(csp) != SP_VER_1) return GS_VALIDATE_ERR_TCP_VER;
  if (CSP_get_2nd_hdr_flag(csp) != SP_2ND_HDR_FLAG_PRESENT)
  {
    // ここではSecondary Headerが必須。
    return GS_VALIDATE_ERR_TCP_2ND_HDR_FLAG;
  }

  if (CSP_get_type(csp) != SP_TYPE_CMD)
  {
    return GS_VALIDATE_ERR_TCP_TYPE_IS_NOT_CMD;
  }

  apid = CSP_get_apid(csp);
  if ( !( apid == APID_MOBC_CMD
       || apid == APID_AOBC_CMD
       || apid == APID_TOBC_CMD ) )
  {
    return GS_VALIDATE_ERR_APID;
  }

  // Sequence Flag が単パケットか確認する
  if (CSP_get_seq_flag(csp) != SP_SEQ_FLAG_SINGLE)
  {
    return GS_VALIDATE_ERR_TCP_SEQ_IS_NOT_SINGLE;
  }

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_fecw_(const uint8_t* data, size_t len)
{
  int i, j;
  uint16_t shift_reg = 0xffff; // 初期値は全bitが1
  uint16_t xor_tap = 0x1021; // LSBは常時0とXORをとっていると考える。

  // データ長だけループ
  for (i = 0; i < len; ++i)
  {
    // MSB位置をshift_regと揃えるため8bit左シフト
    uint16_t tmp = (uint16_t)(data[i] << 8);

    // ビット長だけループ
    for (j = 0; j < 8; ++j)
    {
      // MSB同士のXORを比較
      if ((shift_reg ^ tmp) & 0x8000)
      {
        // 結果が1の場合はシフト+XOR
        shift_reg <<= 1;
        shift_reg ^= xor_tap;
      }
      else
      {
        // 結果が0の場合はシフトのみ
        shift_reg <<= 1;
      }

      // 次ビット評価のためシフト
      tmp <<= 1;
    }
  }

  // データ+FECWが正常なら結果は0となる
  if (shift_reg != 0) return GS_VALIDATE_ERR_FECW_MISSMATCH;

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_ad_cmd_(const TCF* tc_frame)
{
  GS_VALIDATE_ERR ack;
  int seq_diff;

  if (gs_validate_info_.lockout_flag) return GS_VALIDATE_ERR_IN_LOCKOUT;

  ack = GS_check_tcf_contents_(tc_frame);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  seq_diff = TCF_get_frame_seq_num(tc_frame) - gs_validate_info_.type_a_counter;

  if (seq_diff < 0)
  {
    // Sequence Counterの値はmod-256なのでseq_diffの値が負の場合は
    // 256を足して値の範囲を[1, 256]に変換
    seq_diff += 256;
  }

  if (seq_diff == 0)
  {
    // seq_diffが0、すなわちN(R) == V(R)なら正常受信
    // 再送要求フラグのクリアとシーケンス数のインクリメント
    gs_validate_info_.retransmit_flag = 0;
    ++gs_validate_info_.type_a_counter;
  }
  else
  {
    if (seq_diff < gs_validate_info_.positive_window_width)
    {
      gs_validate_info_.retransmit_flag = 1;
      return GS_VALIDATE_ERR_FARM1_POSITIVE_WINDOW_AREA;
    }
    else if (seq_diff < (256 - gs_validate_info_.positive_window_width))
    {
      gs_validate_info_.retransmit_flag = 1;
      return GS_VALIDATE_ERR_FARM1_LOCKOUT_AREA;
    }
    else
    {
      if (seq_diff == 255) return GS_VALIDATE_ERR_FARM1_SAME_NUMBER;
      else return GS_VALIDATE_ERR_FARM1_NEGATIVE_WINDOW_AREA;
    }
  }

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_bc_cmd_(const TCF* tc_frame)
{
  // BCコマンドの種別を判定し、処理する。
  // TCFの構成がAD/BDコマンドに特化した形となっているため、
  // TCSやTCPのデータ構造を読み替えて処理を行っている。
  if (tc_frame->tcs.header[0] == TCF_BC_CMD_CODE_UNLOCK)
  {
    // Unlockコマンドの場合はLockoutとRetransmitフラグをクリア
    gs_validate_info_.lockout_flag = 0;
    gs_validate_info_.retransmit_flag = 0;

    // Type-B Coutnerの値を更新して処理終了
    ++gs_validate_info_.type_b_counter;
  }
  else if ((tc_frame->tcs.header[0] == TCF_BC_CMD_CODE_SET_VR_0)
        && (tc_frame->tcs.tcp.packet[0] == TCF_BC_CMD_CODE_SET_VR_1))
  {
    // SET V(R)コマンドの場合
    if (gs_validate_info_.lockout_flag == 0)
    {
      // Lockout状態でない場合はType-Aカウンタの値を指定値に設定し
      // Retransmitフラグをクリア
      gs_validate_info_.type_a_counter = tc_frame->tcs.tcp.packet[1];
      gs_validate_info_.retransmit_flag = 0;
    }

    // Type-B Coutnerの値を更新して処理終了
    // Lockout状態でもType-B Counterの値は更新する
    ++gs_validate_info_.type_b_counter;
  }
  else
  {
    // 上記以外の場合は不正と判断し異常終了
    return GS_VALIDATE_ERR_INVALID_BC_CMD;
  }

  return GS_VALIDATE_ERR_OK;
}

static GS_VALIDATE_ERR GS_check_bd_cmd_(const TCF* tc_frame)
{
  GS_VALIDATE_ERR ack;
  ack = GS_check_tcf_contents_(tc_frame);
  if (ack != GS_VALIDATE_ERR_OK) return ack;

  ++gs_validate_info_.type_b_counter;

  return GS_VALIDATE_ERR_OK;
}

uint32_t GS_form_clcw(void)
{
  // 値が0固定の項目は変数の初期化で代替し明示的には指定しない
  uint32_t clcw = 0;
  uint32_t val;

  // [FIXME] TRPができたらここも直す．（2021/01/17）
  /*
  // XTRP-A Carrier Lock Status
  if (xtrp1->xtrp_rx_sts.act_monitor.bit.career_lock == 1)
  {
    // Carrier Lock Onの場合はフラグ設定
    clcw |= 0x08000000; // **** 1*** **** **** **** **** **** ****
  }
  */

  // COP in Effect -> COP-1
  clcw |= 0x01000000; // **** **01 **** **** **** **** **** ****

  // [FIXME] TRPができたらここも直す．（2021/01/17）
  /*
  // Sub-Carrier Lock + Rx Bit Rate
  if (xtrp1->xtrp_rx_sts.act_monitor.bit.sub_career_lock == 1)
  {
    // Sub-carrier Lock Onの場合はビットレート別のフラグ設定
    if (xtrp1->xtrp_rx_sts.rx_bitrate == 0)
    {
      // Rx Bitrate 15.625bps (Low)
      clcw |= 0x00004000; // **** **** **** **** 01** **** **** ****
    }
    else if (xtrp1->xtrp_rx_sts.rx_bitrate <= 3)
    {
      // Rx Bitrate 125bps, 62.5bps, 31.25bps (Mid)
      clcw |= 0x00008000; // **** **** **** **** 10** **** **** ****
    }
    else
    {
      // Rx Bitrate 2kbps, 1kbps, 500bps, 250bps (High)
      clcw |= 0x0000c000; // **** **** **** **** 11** **** **** ****
    }
  }
  */

  // Lockout
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

  // Report Value
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
