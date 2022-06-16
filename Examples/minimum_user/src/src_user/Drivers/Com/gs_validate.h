/**
 * @file
 * @brief 受信した TC Transfer Frame, TC  Segment の検証を行う
 * @note  AD, BC, BD コマンドの仕様に関しては
 *        JAXA 設計標準 テレコマンドデータリンクプロトコル設計標準
 *        https://sma.jaxa.jp/TechDoc/Docs/JAXA-JERG-2-401.pdf
 *        実装に関しては
 *        Blue Books: Recommended Standards: TC Space Data Link Protocol
 *        https://public.ccsds.org/Pubs/232x0b4.pdf
 *        をそれぞれ参照した
 */
#ifndef GS_VALIDATE_H_
#define GS_VALIDATE_H_

#include "../../TlmCmd/Ccsds/tc_transfer_frame.h"
#include "../../Library/stdint.h"

/**
 * @enum GS_VALIDATE_ERR
 * @brief gs_validate 関数の返り値
 * @note  uint8_t を想定
 */
typedef enum
{
  GS_VALIDATE_ERR_OK = 0,
  GS_VALIDATE_ERR_TCTF_VER,
  GS_VALIDATE_ERR_TCTF_SCID,
  GS_VALIDATE_ERR_TCTF_VCID,
  GS_VALIDATE_ERR_TCTF_TYPE,
  GS_VALIDATE_ERR_IN_LOCKOUT,
  GS_VALIDATE_ERR_TCS_SEQ_FLAG,
  GS_VALIDATE_ERR_CSP_VER,
  GS_VALIDATE_ERR_CSP_2ND_HDR_FLAG,
  GS_VALIDATE_ERR_APID,
  GS_VALIDATE_ERR_CSP_TYPE_IS_NOT_CMD,
  GS_VALIDATE_ERR_CSP_SEQ_IS_NOT_SINGLE,
  GS_VALIDATE_ERR_FECW_MISSMATCH,
  GS_VALIDATE_ERR_FARM1_POSITIVE_WINDOW_AREA,
  GS_VALIDATE_ERR_FARM1_NEGATIVE_WINDOW_AREA,
  GS_VALIDATE_ERR_FARM1_LOCKOUT_AREA,
  GS_VALIDATE_ERR_INVALID_BC_CMD,
  GS_VALIDATE_ERR_UNKNOWN
} GS_VALIDATE_ERR;

/**
 * @struct GS_ValidateInfo
 * @brief  直近に受信したパケットの検証結果を格納
 */
typedef struct
{
  // FARM-A, B 用のカウンタ値
  uint8_t type_a_counter;
  uint8_t type_b_counter;
  // FARM-1 用の情報
  uint8_t lockout_flag;
  uint8_t retransmit_flag;
  uint8_t positive_window_width;
} GS_ValiateInfo;

extern const GS_ValiateInfo* const gs_validate_info;

/**
 * @brief GS_ValiateInfo の初期化
 * @return void
 */
void GS_validate_init(void);

/**
 * @brief tctf の検証
 * @param[in] tctf: 検証する TC Transfer Frame
 * @return GS_VALIDATE_ERR
 */
GS_VALIDATE_ERR GS_validate_tctf(const TcTransferFrame* tctf);

/**
 * @brief テレメ用の CLCW の作成
 * @return CLCW
 */
uint32_t GS_form_clcw(void);

/**
 * @brief Positive Window Width の設定
 * @param[in] positive_window_width: Positive Window Width
 * @return GS_VALIDATE_ERR
 */
GS_VALIDATE_ERR GS_set_farm_pw(uint8_t positive_window_width);

#endif
