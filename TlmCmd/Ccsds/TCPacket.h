#ifndef TC_PACKET_H_
#define TC_PACKET_H_

#include <stddef.h> // for size_t

#include <src_user/Library/stdint.h>

#define TCP_MAX_LEN          (432u)
#define TCP_PRM_HDR_LEN        (6u)
#define TCP_TLM_2ND_HDR_LEN    (7u)
#define TCP_CMD_2ND_HDR_LEN    (1u)
#define TCP_CMD_USER_HDR_LEN   (8u)

// TCP_MAX_LEN を再定義
// TCP_CMD_DEST_TYPE を定義する
#include <src_user/Settings/TlmCmd/Ccsds/tcpacket_params.h>
// ここで APID を定義する
// 詳細は common_tlm_cmd_packet.h を参照
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>


/**
 * @struct TCP
 * @brief  TCPacket or Space Packet
 */
typedef struct
{
  uint8_t packet[TCP_MAX_LEN];
} TCP;

/**
 * @enum   TCP_VER
 * @brief  TCP version
 * @note   3bit
 */
typedef enum
{
  TCP_VER_1 = 0, // 000b: Version-1
  TCP_VER_UNKNOWN
} TCP_VER;

/**
 * @enum   TCP_TYPE
 * @brief  tlm or cmd を規定
 * @note   0/1の1bit
 */
typedef enum
{
  TCP_TYPE_TLM = 0, // 0b: TELEMETRY
  TCP_TYPE_CMD = 1  // 1b: COMMAND
} TCP_TYPE;

/**
 * @enum   TCP_2ND_HDR_FLAG
 * @brief  TCP の Secondary Header Flag の有無
 * @note   0/1の1bit
 */
typedef enum
{
  TCP_2ND_HDR_ABSENT  = 0, // 0b: Secondary Header Absent
  TCP_2ND_HDR_PRESENT = 1  // 1b: Secondary Header Present
} TCP_2ND_HDR_FLAG;

/**
 * @enum   TCP_SEQ_FLAG
 * @brief  Sequence Flag
 * @note   2bit
 * @note   Packet Sequence Flag for each ADU もこれを用いる．
 *         その場合， component を segment と読み替える．
 */
typedef enum
{
  TCP_SEQ_CONT   = 0, // 00b: Continuation component of higher data structure.
  TCP_SEQ_FIRST  = 1, // 01b: First component of higher data structure.
  TCP_SEQ_LAST   = 2, // 10b: Last component of higher data structure.
  TCP_SEQ_SINGLE = 3  // 11b: Standalone packet.
} TCP_SEQ_FLAG;

/**
 * @enum   TCP_CMD_FMT_ID
 * @brief  Format ID
 * @note   8bit
 */
typedef enum
{
  TCP_CMD_FMT_ID_CONTROL = 0x01, // 01h: Control Command Packet
  TCP_CMD_FMT_ID_USER    = 0x02, // 02h: User Data Command Packet
  TCP_CMD_FMT_ID_MEMORY  = 0x03, // 03h: Memory Write Command Packet
  TCP_CMD_FMT_ID_UNKNOWN
} TCP_CMD_FMT_ID;

/**
 * @enum   TCP_CM
 * @brief  Command Type
 * @note   8bit
 */
typedef enum
{
  TCP_CMD_TYPE_DC = 0x01, // 01h: Discrete Command
  TCP_CMD_TYPE_SM = 0x02, // 02h: Serial Magnitude Command
  TCP_CMD_TYPE_UNKNOWN
} TCP_CMD_TYPE;

/**
 * @enum   TCP_CMD_EXEC_TYPE
 * @brief  コマンド実行種別
 * @note   0x0*を想定（上位4bitは他のC2Aを搭載したボード用に転送するために使うため）
 *         その定義は TCP_DEST_TYPE にある
 *         pure TCPはここが8bitであることに注意
 * @note   TCP_CMD_EXEC_TYPE と CCP_EXEC_TYPE の定義は一致 (正確には、前者は後者に含まれている)
 *         CCP_EXEC_TYPE CCP_get_exec_type(const CTCP* packet) を参照
 * @note   GSからのOBC.TLや，C2A内のCCP_form_tlcはすべてTL0扱いになる
 */
typedef enum
{
  TCP_CMD_EXEC_TYPE_GS  = 0x00, // 00: Ground Station Command
  TCP_CMD_EXEC_TYPE_TL  = 0x01, // 01: Timeline Command
  TCP_CMD_EXEC_TYPE_BC  = 0x02, // 02: Block Command
  TCP_CMD_EXEC_TYPE_RT  = 0x03, // 03: Realtime Command
  TCP_CMD_EXEC_TYPE_UTL = 0x04, // 04: Unixtime Timeline Command
  TCP_CMD_EXEC_TYPE_UNKNOWN
} TCP_CMD_EXEC_TYPE;


// *******************
//  TCP getter/setter
// *******************

/**
 * @brief  TCP ver を取得
 * @param  tcp: TCP
 * @return TCP_VER
 */
TCP_VER TCP_get_ver(const TCP* tcp);

/**
 * @brief  TCP ver を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ver: TCP_VER
 * @return void
 */
void TCP_set_ver(TCP* tcp, TCP_VER ver);

/**
 * @brief  TCP type を取得
 * @param  tcp: TCP
 * @return TCP_TYPE
 */
TCP_TYPE TCP_get_type(const TCP* tcp);

/**
 * @brief  TCP type を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_TYPE
 * @return void
 */
void TCP_set_type(TCP* tcp, TCP_TYPE type);

/**
 * @brief  TCP Secondary Header Flag を取得
 * @param  tcp: TCP
 * @return TCP_2ND_HDR_FLAG
 */
TCP_2ND_HDR_FLAG TCP_get_2nd_hdr_flag(const TCP* tcp);

/**
 * @brief  TCP Secondary Header Flag を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     flag: TCP_2ND_HDR_FLAG
 * @return void
 */
void TCP_set_2nd_hdr_flag(TCP* tcp, TCP_2ND_HDR_FLAG flag);

/**
 * @brief  APID を取得
 * @param  tcp: TCP
 * @return APID
 */
APID TCP_get_apid(const TCP* tcp);

/**
 * @brief  APID を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     apid: APID
 * @return void
 */
void TCP_set_apid(TCP* tcp, APID apid);

/**
 * @brief  Sequence Flag を取得
 * @param  tcp: TCP
 * @return TCP_SEQ_FLAG
 */
TCP_SEQ_FLAG TCP_get_seq_flag(const TCP* tcp);

/**
 * @brief  Sequence Flag を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     flag: TCP_SEQ_FLAG
 * @return void
 */
void TCP_set_seq_flag(TCP* tcp, TCP_SEQ_FLAG flag);

/**
 * @brief  Sequence Count を取得
 * @param  tcp: TCP
 * @return Sequence Count
 */
uint16_t TCP_get_seq_cnt(const TCP* tcp);

/**
 * @brief  Sequence Count を設定
 * @param[in,out] tcp: TCP
 * @param[in]     cnt: Sequence Count
 * @return void
 */
void TCP_set_seq_cnt(TCP* tcp, uint16_t cnt);

/**
 * @brief  Packet Length を取得
 * @param  tcp: TCP
 * @return Packet Length
 */
uint16_t TCP_get_packet_len(const TCP* tcp);

/**
 * @brief  Packet Length を設定
 * @note   Packet Length とは space packet のPACKET DATA FIELD 長であり， PRIMARY HEADER を含まない
 *         ( https://public.ccsds.org/Pubs/133x0b2e1.pdf )
 * @param[in,out] tcp: TCP
 * @param[in]     len: Packet Length
 * @return void
 */
void TCP_set_packet_len(TCP* tcp, uint16_t len);

/**
 * @brief  TCP をコピー
 * @note   高速化のためすすべてではなく必要最低限しかコピーしない場合
 * @param[in,out] dest: コピー先 TCP
 * @param[in]     src:  コピー元 TCP
 * @return void
 */
void TCP_copy_packet(TCP* dest, const TCP* src);

// ***********************
//  TCP CMD getter/setter
// ***********************

/**
 * @brief  Format ID を取得
 * @param  tcp: TCP
 * @return TCP_CMD_FMT_ID
 */
TCP_CMD_FMT_ID TCP_CMD_get_fmt_id(const TCP* tcp);

/**
 * @brief  Format ID を設定
 * @param[in,out] tcp: TCP
 * @param[in]     id:  TCP_CMD_FMT_ID
 * @return void
 */
void TCP_CMD_set_fmt_id(TCP* tcp, TCP_CMD_FMT_ID id);

/**
 * @brief  Command Type を取得
 * @param  tcp: TCP
 * @return TCP_CMD_TYPE
 */
TCP_CMD_TYPE TCP_CMD_get_type(const TCP* tcp);

/**
 * @brief  Command Type を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_TYPE
 * @return void
 */
void TCP_CMD_set_type(TCP* tcp, TCP_CMD_TYPE type);

/**
 * @brief  channel ID を取得
 * @param  tcp: TCP
 * @return channel ID
 */
uint16_t TCP_CMD_get_channel_id(const TCP* tcp);

/**
 * @brief  channel ID を設定
 * @param[in,out] tcp: TCP
 * @param[in]     id:  channel ID
 * @return void
 */
void TCP_CMD_set_channel_id(TCP* tcp, uint16_t id);

/**
 * @brief  EXEC_TYPE を取得
 * @param  tcp: TCP
 * @return TCP_CMD_EXEC_TYPE
 */
TCP_CMD_EXEC_TYPE TCP_CMD_get_exec_type(const TCP* tcp);

/**
 * @brief  EXEC_TYPE を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_EXEC_TYPE
 * @return void
 */
void TCP_CMD_set_exec_type(TCP* tcp, TCP_CMD_EXEC_TYPE type);

/**
 * @brief  CMD_DEST_TYPE を取得
 * @param  tcp: TCP
 * @return TCP_CMD_DEST_TYPE
 * @note   2021/01/22に TCP_CMD_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
TCP_CMD_DEST_TYPE TCP_CMD_get_dest_type(const TCP* tcp);

/**
 * @brief  CMD_DEST_TYPE を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     type: TCP_CMD_DEST_TYPE
 * @return void
 * @note   2021/01/22に TCP_CMD_EXEC_TYPE の上位4bitを拝借する形で追加実装
 */
void TCP_CMD_set_dest_type(TCP* tcp, TCP_CMD_DEST_TYPE type);

/**
 * @brief  Time Indicator (TI) を取得
 * @param  tcp: TCP
 * @return TI
 */
uint32_t TCP_CMD_get_ti(const TCP* tcp);

/**
 * @brief  Time Indicator (TI) を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ti:  TI
 * @return void
 */
void TCP_CMD_set_ti(TCP* tcp, uint32_t ti);

/**
 * @brief  先頭のパラメタのポインタを取得
 * @param  tcp: TCP
 * @return *param_head
 */
const uint8_t* TCP_CMD_get_param_head(const TCP* tcp);

/**
 * @brief  パラメタを設定
 * @param[in,out] tcp:   TCP
 * @param[in]     param: パラメタ配列のポインタ
 * @param[in]     len:   パラメタ長
 * @return void
 */
void TCP_CMD_set_param(TCP* tcp, const uint8_t* param, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] tcp:  TCP
 * @return void
 */
void TCP_CMD_set_common_hdr(TCP* tcp);


// ***********************
//  TCP TLM getter/setter
// ***********************

/**
 * @brief  Time Indicator (TI) を取得
 * @param  tcp: TCP
 * @return TI
 */
uint32_t TCP_TLM_get_ti(const TCP* tcp);

/**
 * @brief  Time Indicator (TI) を設定
 * @param[in,out] tcp: TCP
 * @param[in]     ti:  TI
 * @return void
 */
void TCP_TLM_set_ti(TCP* tcp, uint32_t ti);

/**
 * @brief  category を取得
 * @param  tcp: TCP
 * @return category
 */
uint8_t TCP_TLM_get_category(const TCP* tcp);

/**
 * @brief  category を設定
 * @param[in,out] tcp: TCP
 * @param[in]     category: category
 * @return void
 */
void TCP_TLM_set_category(TCP* tcp, uint8_t category);

/**
 * @brief  packet id を取得
 * @param  tcp: TCP
 * @return packet id
 */
uint8_t TCP_TLM_get_packet_id(const TCP* tcp);

/**
 * @brief  packet id を設定
 * @param[in,out] tcp: TCP
 * @param[in]     id:  packet id
 * @return void
 */
void TCP_TLM_set_packet_id(TCP* tcp, uint8_t id);

/**
 * @brief  Packet Sequence Flag for each ADU を取得
 * @param  tcp: TCP
 * @return TCP_SEQ_FLAG
 */
TCP_SEQ_FLAG TCP_TLM_get_adu_seq_flag(const TCP* tcp);

/**
 * @brief  Packet Sequence Flag for each ADU を設定
 * @param[in,out] tcp:  TCP
 * @param[in]     flag: TCP_SEQ_FLAG
 * @return void
 */
void TCP_TLM_set_adu_seq_flag(TCP* tcp, TCP_SEQ_FLAG flag);

/**
 * @brief  ADU Count を取得
 * @param  tcp: TCP
 * @return ADU Count
 */
uint8_t TCP_TLM_get_adu_cnt(const TCP* tcp);

/**
 * @brief  ADU Count を設定
 * @param[in,out] tcp: TCP
 * @param[in]     cnt: ADU Count
 * @return void
 */
void TCP_TLM_set_adu_cnt(TCP* tcp, uint8_t cnt);

/**
 * @brief  Packet Sequence Count for each ADU を取得
 * @param  tcp: TCP
 * @return Packet Sequence Count for each ADU
 */
uint16_t TCP_TLM_get_adu_seq_cnt(const TCP* tcp);

/**
 * @brief  Packet Sequence Count for each ADU を設定
 * @param[in,out] tcp: TCP
 * @param[in]     cnt: Packet Sequence Count for each ADU
 * @return void
 */
void TCP_TLM_set_adu_seq_cnt(TCP* tcp, uint16_t cnt);

/**
 * @brief  ADU Length を取得
 * @param  tcp: TCP
 * @return ADU Length
 */
uint32_t TCP_TLM_get_adu_len(const TCP* tcp);

/**
 * @brief  ADU Length を設定
 * @param[in,out] tcp: TCP
 * @param[in]     len: ADU Length
 * @return void
 * @note   24 bitなため，最上位8bitは切り捨てられる
 */
void TCP_TLM_set_adu_len(TCP* tcp, uint32_t len);

/**
 * @brief  TCP全体の（ Primary Header を含んだ） Packet長を取得
 * @param[in,out] tcp: TCP
 * @param[in]     len: Packet Length
 * @return void
 */
uint16_t TCP_TLM_get_packet_len(const TCP* packet);

/**
 * @brief  ADU分割なしの場合のUser Data先頭のポインタを取得
 * @param  tcp: TCP
 * @return *param_head
 * @note   この関数で取得したポインタ経由でtlmのデータをセットする事があるため，constをつけていない
 */
uint8_t* TCP_TLM_get_user_data_head(TCP* packet);

/**
 * @brief  Primary Header を構築
 * @param[in,out] tcp:  TCP
 * @param[in]     apid: TCP_APID
 * @param[in]     len:  Packet Length
 * @return void
 */
void TCP_TLM_setup_primary_hdr(TCP* tcp, APID apid, uint16_t len);

/**
 * @brief  共通部分の Header を構築
 * @param[in,out] tcp:  TCP
 * @return void
 */
void TCP_TLM_set_common_hdr(TCP* tcp);

/**
 * @brief  TCP全体を0x00クリアし，fill_sizeに等しいサイズのパケットを生成する
 * @param[in,out] tcp: TCP
 * @param[in]     fill_size: Packet size
 * @return void
 */
void TCP_TLM_setup_fill_packet(TCP* tcp, uint16_t fill_size);

#endif
