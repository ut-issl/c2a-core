#pragma section REPRO
#include "TCPacket.h"

#include <string.h> // for memcpy

#include <src_core/CmdTlm/common_tlm_cmd_packet.h>
#include "../command_definitions.h" // for Cmd_CODE_EXECUTE_APP
#include <src_core/CmdTlm/packet_handler.h> // for TL_CMD_LIST_LINE
#include <src_core/CmdTlm/block_command_table.h> // for BCT_MAX_BLOCK
#include <src_core/Library/endian_memcpy.h>

/**
 * @struct TCP_PacketParams
 * @brief  packetからデータを抜き取るときのパラメタ
 */
typedef struct
{
  uint16_t pos;
  uint8_t  mask;
  uint8_t  offset;
} TCP_PacketParams;

static const TCP_PacketParams TCP_pp_ver_              = { 0, 0xe0, 5};  // 11100000b
static const TCP_PacketParams TCP_pp_type_             = { 0, 0x10, 4};  // 00010000b
static const TCP_PacketParams TCP_pp_2nd_hdr_flag_     = { 0, 0x08, 3};  // 00001000b
static const TCP_PacketParams TCP_pp_apid_             = { 0, 0x07, 0};  // 00000111b    // 2byteにまたがることに注意
static const TCP_PacketParams TCP_pp_seq_flag_         = { 2, 0xc0, 6};  // 11000000b
static const TCP_PacketParams TCP_pp_seq_cnt_          = { 2, 0x3f, 0};  // 00111111b    // 2byteにまたがることに注意
static const TCP_PacketParams TCP_pp_packet_len_       = { 4, 0xff, 0};  // 11111111b    // 2byteにまたがることに注意

static const TCP_PacketParams TCP_CMD_pp_fmt_id_       = { 6, 0xff, 0};  // 11111111b
static const TCP_PacketParams TCP_CMD_pp_type_         = { 7, 0xff, 0};  // 11111111b
static const TCP_PacketParams TCP_CMD_pp_ch_id_        = { 8, 0xff, 0};  // 11111111b    // 2byteにまたがることに注意
static const TCP_PacketParams TCP_CMD_pp_exec_type_    = {10, 0x0f, 0};  // 00001111b    // TCP_CMD_DEST_TYPE と共用していることに注意
static const TCP_PacketParams TCP_CMD_pp_dest_type_    = {10, 0xf0, 0};  // 11110000b    // TCP_CMD_EXEC_TYPE と共用していることに注意
static const TCP_PacketParams TCP_CMD_pp_ti_           = {11, 0xff, 0};  // 11111111b    // 4byteにまたがることに注意

static const TCP_PacketParams TCP_TLM_pp_ti_           = { 6, 0xff, 0};  // 11111111b    // 4byteにまたがることに注意
static const TCP_PacketParams TCP_TLM_pp_category_     = {10, 0xff, 0};  // 11111111b
static const TCP_PacketParams TCP_TLM_pp_packet_id_    = {11, 0xff, 0};  // 11111111b
static const TCP_PacketParams TCP_TLM_pp_adu_seq_flag_ = {12, 0xc0, 6};  // 11000000b
static const TCP_PacketParams TCP_TLM_pp_adu_cnt_      = {12, 0x3f, 0};  // 00111111b
static const TCP_PacketParams TCP_TLM_pp_adu_seq_cnt_  = {13, 0xff, 0};  // 11111111b    // 2byteにまたがることに注意
static const TCP_PacketParams TCP_TLM_pp_adu_len_      = {15, 0xff, 0};  // 11111111b    // 3byteにまたがることに注意


// *******************
//  TCP getter/setter
// *******************

TCP_VER TCP_get_ver(const TCP* tcp)
{
  TCP_VER ver = (TCP_VER)((tcp->packet[TCP_pp_ver_.pos] & TCP_pp_ver_.mask) >> TCP_pp_ver_.offset);

  switch (ver)
  {
  case TCP_VER_1:
    return ver;
  default:
    return TCP_VER_UNKNOWN;
  }
}

void TCP_set_ver(TCP* tcp, TCP_VER ver)
{
  uint8_t val = (uint8_t)(ver << TCP_pp_ver_.offset);

  tcp->packet[TCP_pp_ver_.pos] &= (uint8_t)(~TCP_pp_ver_.mask);
  tcp->packet[TCP_pp_ver_.pos] |= (uint8_t)(val & TCP_pp_ver_.mask);
}

TCP_TYPE TCP_get_type(const TCP* tcp)
{
  return (TCP_TYPE)((tcp->packet[TCP_pp_type_.pos] & TCP_pp_type_.mask) >> TCP_pp_type_.offset);
}

void TCP_set_type(TCP* tcp, TCP_TYPE type)
{
  uint8_t val = (uint8_t)(type << TCP_pp_type_.offset);

  tcp->packet[TCP_pp_type_.pos] &= (uint8_t)(~TCP_pp_type_.mask);
  tcp->packet[TCP_pp_type_.pos] |= (uint8_t)(val & TCP_pp_type_.mask);
}

TCP_2ND_HDR_FLAG TCP_get_2nd_hdr_flag(const TCP* tcp)
{
  return (TCP_2ND_HDR_FLAG)((tcp->packet[TCP_pp_2nd_hdr_flag_.pos] & TCP_pp_2nd_hdr_flag_.mask) >> TCP_pp_2nd_hdr_flag_.offset);
}

void TCP_set_2nd_hdr_flag(TCP* tcp, TCP_2ND_HDR_FLAG flag)
{
  uint8_t val = (uint8_t)(flag << TCP_pp_2nd_hdr_flag_.offset);

  tcp->packet[TCP_pp_2nd_hdr_flag_.pos] &= (uint8_t)(~TCP_pp_2nd_hdr_flag_.mask);
  tcp->packet[TCP_pp_2nd_hdr_flag_.pos] |= (uint8_t)(val & TCP_pp_2nd_hdr_flag_.mask);
}

TCP_APID TCP_get_apid(const TCP* tcp)
{
  TCP_APID apid = (TCP_APID)(tcp->packet[TCP_pp_apid_.pos] & TCP_pp_apid_.mask);
  apid = (TCP_APID)(apid << 8);
  apid = (TCP_APID)(apid + tcp->packet[TCP_pp_apid_.pos + 1]);

  switch (apid)
  {
  case TCP_APID_MOBC_CMD:         // FALLTHROUGH
  case TCP_APID_AOBC_CMD:         // FALLTHROUGH
  case TCP_APID_TOBC_CMD:         // FALLTHROUGH
  case TCP_APID_MIS_TLM:          // FALLTHROUGH
  case TCP_APID_DUMP_TLM:
    return apid;

  default:
    return TCP_APID_UNKNOWN;
  }
}

void TCP_set_apid(TCP* tcp, TCP_APID apid)
{
  uint8_t val = (uint8_t)((apid >> 8) & TCP_pp_apid_.mask);

  tcp->packet[TCP_pp_apid_.pos] &= (uint8_t)(~TCP_pp_apid_.mask);
  tcp->packet[TCP_pp_apid_.pos] |= (uint8_t)(val & TCP_pp_apid_.mask);

  val = (uint8_t)(apid & 0xff);
  tcp->packet[TCP_pp_apid_.pos + 1] = val;
}

TCP_SEQ_FLAG TCP_get_seq_flag(const TCP* tcp)
{
  return (TCP_SEQ_FLAG)((tcp->packet[TCP_pp_seq_flag_.pos] & TCP_pp_seq_flag_.mask) >> TCP_pp_seq_flag_.offset);
}

void TCP_set_seq_flag(TCP* tcp,
                      TCP_SEQ_FLAG flag)
{
  uint8_t val = (uint8_t)(flag << TCP_pp_seq_flag_.offset);

  tcp->packet[TCP_pp_seq_flag_.pos] &= (uint8_t)(~TCP_pp_seq_flag_.mask);
  tcp->packet[TCP_pp_seq_flag_.pos] |= (uint8_t)(val & TCP_pp_seq_flag_.mask);
}

uint16_t TCP_get_seq_cnt(const TCP* tcp)
{
  uint16_t seq_cnt = (uint16_t)(tcp->packet[TCP_pp_seq_cnt_.pos] & TCP_pp_seq_cnt_.mask);
  seq_cnt <<= 8;
  seq_cnt += tcp->packet[TCP_pp_seq_cnt_.pos + 1];

  return seq_cnt;
}

void TCP_set_seq_cnt(TCP* tcp, uint16_t cnt)
{
  uint8_t val = (uint8_t)((cnt >> 8) & TCP_pp_seq_cnt_.mask);

  tcp->packet[TCP_pp_seq_cnt_.pos] &= (uint8_t)(~TCP_pp_seq_cnt_.mask);
  tcp->packet[TCP_pp_seq_cnt_.pos] |= (uint8_t)(val & TCP_pp_seq_cnt_.mask);

  val = (uint8_t)(cnt & 0xff);
  tcp->packet[TCP_pp_seq_cnt_.pos + 1] = val;
}

uint16_t TCP_get_packet_len(const TCP* tcp)
{
  uint16_t packet_len = (uint16_t)(tcp->packet[TCP_pp_packet_len_.pos] & TCP_pp_packet_len_.mask);
  packet_len <<= 8;
  packet_len += tcp->packet[TCP_pp_packet_len_.pos + 1];

  // TCPacketのLengthは0起算表記なので1起算に変換して返す
  return (uint16_t)(packet_len + 1);
}

void TCP_set_packet_len(TCP* tcp, uint16_t len)
{
  uint16_t z_origin = (uint16_t)(len - 1);  // CCSDSのデータ長は0起算表記なので1起算を変換
  uint8_t val = (uint8_t)(z_origin >> 8 & TCP_pp_packet_len_.mask);

  tcp->packet[TCP_pp_packet_len_.pos] &= (uint8_t)(~TCP_pp_packet_len_.mask);
  tcp->packet[TCP_pp_packet_len_.pos] |= (uint8_t)(val & TCP_pp_packet_len_.mask);

  val = (uint8_t)(z_origin & 0xff);
  tcp->packet[TCP_pp_packet_len_.pos + 1] = val;
}

void TCP_copy_packet(TCP* dest, const TCP* src)
{
  uint16_t cmd_packet_len = (uint16_t)(TCP_get_packet_len(src) + TCP_PRM_HDR_LEN);
  // 高速化のために，構造体のすべてをコピーせず，必要最低限しかコピーしていない
  memcpy(dest->packet, src->packet, (size_t)cmd_packet_len);
}


// ***********************
//  TCP CMD getter/setter
// ***********************

TCP_CMD_FMT_ID TCP_CMD_get_fmt_id(const TCP* tcp)
{
  TCP_CMD_FMT_ID fmt_id = (TCP_CMD_FMT_ID)((tcp->packet[TCP_CMD_pp_fmt_id_.pos] & TCP_CMD_pp_fmt_id_.mask) >> TCP_CMD_pp_fmt_id_.offset);

  switch (fmt_id)
  {
  case TCP_CMD_FMT_ID_CONTROL: // FALL THROUGH
  case TCP_CMD_FMT_ID_USER:    // FALL THROUGH
  case TCP_CMD_FMT_ID_MEMORY:
    return fmt_id;
  default:
    return TCP_CMD_FMT_ID_UNKNOWN;
  }
}

void TCP_CMD_set_fmt_id(TCP* tcp, TCP_CMD_FMT_ID id)
{
  uint8_t val = (uint8_t)(id << TCP_CMD_pp_fmt_id_.offset);

  tcp->packet[TCP_CMD_pp_fmt_id_.pos] &= (uint8_t)(~TCP_CMD_pp_fmt_id_.mask);
  tcp->packet[TCP_CMD_pp_fmt_id_.pos] |= (uint8_t)(val & TCP_CMD_pp_fmt_id_.mask);
}

TCP_CMD_TYPE TCP_CMD_get_type(const TCP* tcp)
{
  TCP_CMD_TYPE type = (TCP_CMD_TYPE)((tcp->packet[TCP_CMD_pp_type_.pos] & TCP_CMD_pp_type_.mask) >> TCP_CMD_pp_type_.offset);

  switch (type)
  {
  case TCP_CMD_TYPE_DC: // FALL THROUGH
  case TCP_CMD_TYPE_SM:
    return type;
  default:
    return TCP_CMD_TYPE_UNKNOWN;
  }
}

void TCP_CMD_set_type(TCP* tcp, TCP_CMD_TYPE type)
{
  uint8_t val = (uint8_t)(type << TCP_CMD_pp_type_.offset);

  tcp->packet[TCP_CMD_pp_type_.pos] &= (uint8_t)(~TCP_CMD_pp_type_.mask);
  tcp->packet[TCP_CMD_pp_type_.pos] |= (uint8_t)(val & TCP_CMD_pp_type_.mask);
}

uint16_t TCP_CMD_get_channel_id(const TCP* tcp)
{
  uint16_t id = (uint16_t)(tcp->packet[TCP_CMD_pp_ch_id_.pos] & TCP_CMD_pp_ch_id_.mask);
  id <<= 8;
  id += tcp->packet[TCP_CMD_pp_ch_id_.pos + 1];

  return id;
}

void TCP_CMD_set_channel_id(TCP* tcp, uint16_t id)
{
  uint8_t val = (uint8_t)((id >> 8) & TCP_CMD_pp_ch_id_.mask);

  tcp->packet[TCP_CMD_pp_ch_id_.pos] &= (uint8_t)(~TCP_CMD_pp_ch_id_.mask);
  tcp->packet[TCP_CMD_pp_ch_id_.pos] |= (uint8_t)(val & TCP_CMD_pp_ch_id_.mask);

  val = (uint8_t)(id & 0xff);
  tcp->packet[TCP_CMD_pp_ch_id_.pos + 1] = val;
}

// 2021/01/22 TCP_CMD_DEST_TYPE の誕生に伴い，下位4bitのみのmaskになった
// これがpure TCPとの違い
TCP_CMD_EXEC_TYPE TCP_CMD_get_exec_type(const TCP* tcp)
{
  TCP_CMD_EXEC_TYPE exec_type = (TCP_CMD_EXEC_TYPE)((tcp->packet[TCP_CMD_pp_exec_type_.pos] & TCP_CMD_pp_exec_type_.mask) >>
                                                    TCP_CMD_pp_exec_type_.offset);

  switch (exec_type)
  {
  case TCP_CMD_EXEC_TYPE_GS: // FALL THROUGH
  case TCP_CMD_EXEC_TYPE_TL: // FALL THROUGH
  case TCP_CMD_EXEC_TYPE_UTL: // FALL THROUGH
  case TCP_CMD_EXEC_TYPE_BC: // FALL THROUGH
  case TCP_CMD_EXEC_TYPE_RT:
    return exec_type;
  default:
    return TCP_CMD_EXEC_TYPE_UNKNOWN;
  }
}

// 2021/01/22 TCP_CMD_DEST_TYPE の誕生に伴い，下位4bitのみのmaskになった
// これがpure TCPとの違い
void TCP_CMD_set_exec_type(TCP* tcp, TCP_CMD_EXEC_TYPE type)
{
  uint8_t val = (uint8_t)(type << TCP_CMD_pp_exec_type_.offset);

  tcp->packet[TCP_CMD_pp_exec_type_.pos] &= (uint8_t)(~TCP_CMD_pp_exec_type_.mask);
  tcp->packet[TCP_CMD_pp_exec_type_.pos] |= (uint8_t)(val & TCP_CMD_pp_exec_type_.mask);
}

TCP_CMD_DEST_TYPE TCP_CMD_get_dest_type(const TCP* tcp)
{
  TCP_CMD_DEST_TYPE dest_type = (TCP_CMD_DEST_TYPE)((tcp->packet[TCP_CMD_pp_dest_type_.pos] & TCP_CMD_pp_dest_type_.mask) >>
                                                    TCP_CMD_pp_dest_type_.offset);

  switch (dest_type)
  {
  case TCP_CMD_DEST_TYPE_TO_ME:   // FALL THROUGH
  case TCP_CMD_DEST_TYPE_TO_MOBC: // FALL THROUGH
  case TCP_CMD_DEST_TYPE_TO_AOBC: // FALL THROUGH
  case TCP_CMD_DEST_TYPE_TO_TOBC:
    return dest_type;
  default:
    return TCP_CMD_DEST_TYPE_TO_UNKOWN;
  }
}

void TCP_CMD_set_dest_type(TCP* tcp, TCP_CMD_DEST_TYPE type)
{
  uint8_t val = (uint8_t)(type << TCP_CMD_pp_dest_type_.offset);

  tcp->packet[TCP_CMD_pp_dest_type_.pos] &= (uint8_t)(~TCP_CMD_pp_dest_type_.mask);
  tcp->packet[TCP_CMD_pp_dest_type_.pos] |= (uint8_t)(val & TCP_CMD_pp_dest_type_.mask);
}

uint32_t TCP_CMD_get_ti(const TCP* tcp)
{
  uint32_t ti = tcp->packet[TCP_CMD_pp_ti_.pos];
  ti <<= 8;
  ti += tcp->packet[TCP_CMD_pp_ti_.pos + 1];
  ti <<= 8;
  ti += tcp->packet[TCP_CMD_pp_ti_.pos + 2];
  ti <<= 8;
  ti += tcp->packet[TCP_CMD_pp_ti_.pos + 3];

  return ti;
}

void TCP_CMD_set_ti(TCP* tcp, uint32_t ti)
{
  tcp->packet[TCP_CMD_pp_ti_.pos]     = (uint8_t)((ti >> 24) & 0xff);
  tcp->packet[TCP_CMD_pp_ti_.pos + 1] = (uint8_t)((ti >> 16) & 0xff);
  tcp->packet[TCP_CMD_pp_ti_.pos + 2] = (uint8_t)((ti >>  8) & 0xff);
  tcp->packet[TCP_CMD_pp_ti_.pos + 3] = (uint8_t)(ti & 0xff);
}

const uint8_t* TCP_CMD_get_param_head(const TCP* tcp)
{
  return &(tcp->packet[TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN]);
}

void TCP_CMD_set_param(TCP* tcp, const uint8_t* param, uint16_t len)
{
  // パラメータをパケットにコピー
  // paramは既に成形されているのでここは endian_memcpy ではなく memcpy (paramの数, 種類などがこの関数からは不明)
  memcpy(&(tcp->packet[TCP_PRM_HDR_LEN + TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN]), param, (size_t)len);

  // パラメータ長に合わせパケット長を設定
  // パケット長はヘッダ分が加算される
  TCP_set_packet_len(tcp, (uint16_t)(TCP_CMD_2ND_HDR_LEN + TCP_CMD_USER_HDR_LEN + len));
}

void TCP_CMD_set_common_hdr(TCP* tcp)
{
  // ここではVersion NumberはVersion-1固定
  TCP_set_ver(tcp, TCP_VER_1);
  // Command PacketなのでTypeはTelecommand固定
  TCP_set_type(tcp, TCP_TYPE_CMD);
  // ここではSecondary Headerは必須
  TCP_set_2nd_hdr_flag(tcp, TCP_2ND_HDR_PRESENT);
  // APID
  TCP_set_apid(tcp, CTCP_MY_DST_ID);
  // ここではSequence FlagはStandalone Packetに固定
  TCP_set_seq_flag(tcp, TCP_SEQ_SINGLE);
  // ここではSequence Countは0固定
  TCP_set_seq_cnt(tcp, 0);
  // ここではFormat IDはControl Command Packet固定
  TCP_CMD_set_fmt_id(tcp, TCP_CMD_FMT_ID_CONTROL);
  // ここではCommand TypeはSerial Magnitude固定
  TCP_CMD_set_type(tcp, TCP_CMD_TYPE_SM);
}


// ***********************
//  TCP TLM getter/setter
// ***********************

uint32_t TCP_TLM_get_ti(const TCP* tcp)
{
  uint32_t ti = tcp->packet[TCP_TLM_pp_ti_.pos];
  ti <<= 8;
  ti += tcp->packet[TCP_TLM_pp_ti_.pos + 1];
  ti <<= 8;
  ti += tcp->packet[TCP_TLM_pp_ti_.pos + 2];
  ti <<= 8;
  ti += tcp->packet[TCP_TLM_pp_ti_.pos + 3];

  return ti;
}

void TCP_TLM_set_ti(TCP* tcp, uint32_t ti)
{
  tcp->packet[TCP_TLM_pp_ti_.pos]     = (uint8_t)((ti >> 24) & 0xff);
  tcp->packet[TCP_TLM_pp_ti_.pos + 1] = (uint8_t)((ti >> 16) & 0xff);
  tcp->packet[TCP_TLM_pp_ti_.pos + 2] = (uint8_t)((ti >>  8) & 0xff);
  tcp->packet[TCP_TLM_pp_ti_.pos + 3] = (uint8_t)(ti & 0xff);
}

uint8_t TCP_TLM_get_category(const TCP* tcp)
{
  return (uint8_t)(tcp->packet[TCP_TLM_pp_category_.pos]);
}

void TCP_TLM_set_category(TCP* tcp, uint8_t category)
{
  tcp->packet[TCP_TLM_pp_category_.pos] = category;
}

uint8_t TCP_TLM_get_packet_id(const TCP* tcp)
{
  return (uint8_t)(tcp->packet[TCP_TLM_pp_packet_id_.pos]);
}

void TCP_TLM_set_packet_id(TCP* tcp, uint8_t id)
{
  tcp->packet[TCP_TLM_pp_packet_id_.pos] = id;
}


TCP_SEQ_FLAG TCP_TLM_get_adu_seq_flag(const TCP* tcp)
{
  return (TCP_SEQ_FLAG)((tcp->packet[TCP_TLM_pp_adu_seq_flag_.pos] & TCP_TLM_pp_adu_seq_flag_.mask) >> TCP_TLM_pp_adu_seq_flag_.offset);
}

void TCP_TLM_set_adu_seq_flag(TCP* tcp, TCP_SEQ_FLAG flag)
{
  uint8_t val = (uint8_t)(flag << TCP_TLM_pp_adu_seq_flag_.offset);

  tcp->packet[TCP_TLM_pp_adu_seq_flag_.pos] &= (uint8_t)(~TCP_TLM_pp_adu_seq_flag_.mask);
  tcp->packet[TCP_TLM_pp_adu_seq_flag_.pos] |= (uint8_t)(val & TCP_TLM_pp_adu_seq_flag_.mask);
}

uint8_t TCP_TLM_get_adu_cnt(const TCP* tcp)
{
  return (uint8_t)((tcp->packet[TCP_TLM_pp_adu_cnt_.pos] & TCP_TLM_pp_adu_cnt_.mask) >> TCP_TLM_pp_adu_cnt_.offset);
}

void TCP_TLM_set_adu_cnt(TCP* tcp, uint8_t cnt)
{
  uint8_t val = (uint8_t)(cnt << TCP_TLM_pp_adu_cnt_.offset);

  tcp->packet[TCP_TLM_pp_adu_cnt_.pos] &= (uint8_t)(~TCP_TLM_pp_adu_cnt_.mask);
  tcp->packet[TCP_TLM_pp_adu_cnt_.pos] |= (uint8_t)(val & TCP_TLM_pp_adu_cnt_.mask);
}

uint16_t TCP_TLM_get_adu_seq_cnt(const TCP* tcp)
{
  uint16_t cnt = tcp->packet[TCP_TLM_pp_adu_seq_cnt_.pos];
  cnt <<= 8;
  cnt += tcp->packet[TCP_TLM_pp_adu_seq_cnt_.pos + 1];

  return cnt;
}

void TCP_TLM_set_adu_seq_cnt(TCP* tcp, uint16_t cnt)
{
  tcp->packet[TCP_TLM_pp_adu_seq_cnt_.pos]     = (uint8_t)((cnt >>  8) & 0xff);
  tcp->packet[TCP_TLM_pp_adu_seq_cnt_.pos + 1] = (uint8_t)(cnt & 0xff);
}

uint32_t TCP_TLM_get_adu_len(const TCP* tcp)
{
  uint32_t len = tcp->packet[TCP_TLM_pp_adu_len_.pos];
  len <<= 8;
  len += tcp->packet[TCP_TLM_pp_adu_len_.pos + 1];
  len <<= 8;
  len += tcp->packet[TCP_TLM_pp_adu_len_.pos + 2];

  // TCPacketのADU Lengthは0起算なので1起算に変換して返す
  return len + 1;
}

void TCP_TLM_set_adu_len(TCP* tcp, uint32_t len)
{
  uint32_t z_origin = len - 1; // 1起算の値を0起算に変換

  // 24bit長なので最上位8bitは切り捨てられられる
  tcp->packet[TCP_TLM_pp_adu_len_.pos]     = (uint8_t)((z_origin >> 16) & 0xff);
  tcp->packet[TCP_TLM_pp_adu_len_.pos + 1] = (uint8_t)((z_origin >>  8) & 0xff);
  tcp->packet[TCP_TLM_pp_adu_len_.pos + 2] = (uint8_t)(z_origin & 0xff);
}

uint16_t TCP_TLM_get_packet_len(const TCP* packet)
{
  // User Data長 + TCP Primary Header長
  return (uint16_t)(TCP_get_packet_len(packet) + TCP_PRM_HDR_LEN);
}

uint8_t* TCP_TLM_get_user_data_head(TCP* tcp)
{
  return &(tcp->packet[TCP_PRM_HDR_LEN + TCP_TLM_2ND_HDR_LEN]);
}

void TCP_TLM_setup_primary_hdr(TCP* tcp, TCP_APID apid, uint16_t len)
{
  TCP_TLM_set_common_hdr(tcp);
  TCP_set_apid(tcp, apid);
  TCP_set_packet_len(tcp, len);
}

void TCP_TLM_set_common_hdr(TCP* tcp)
{
  // ここではVersion NumberはVersion-1固定
  TCP_set_ver(tcp, TCP_VER_1);
  // Telemetry PacketなのでTypeはTelemetry固定
  TCP_set_type(tcp, TCP_TYPE_TLM);
  // ここではSecondary Headerは必須
  TCP_set_2nd_hdr_flag(tcp, TCP_2ND_HDR_PRESENT);
  // ここではSequence FlagはStandalone Packetに固定
  TCP_set_seq_flag(tcp, TCP_SEQ_SINGLE);
  // ここではSequence Countは0固定
  TCP_set_seq_cnt(tcp, 0);
}

void TCP_TLM_setup_fill_packet(TCP* tcp, uint16_t fill_size)
{
  // パケットの内容を0クリア。
  memset(tcp->packet, 0x00, TCP_MAX_LEN);

  if (fill_size > TCP_PRM_HDR_LEN)
  {
    // 穴埋め領域がTC Packet Primary Header長より大きい場合。
    // 穴埋め領域にピッタリハマるFill Packetを生成する。
    TCP_TLM_setup_primary_hdr(tcp,
                              TCP_APID_FILL_PKT,
                              (uint16_t)(fill_size - TCP_PRM_HDR_LEN));
  }
  else
  {
    // 穴埋め領域がTC Packet Primary Header長より小さい場合。
    // 領域ピッタリのFill Packetの生成は不可能なので、データ長1
    // のFill Packetを生成する。
    TCP_TLM_setup_primary_hdr(tcp,
                              TCP_APID_FILL_PKT,
                              1);
  }
}

#pragma section
