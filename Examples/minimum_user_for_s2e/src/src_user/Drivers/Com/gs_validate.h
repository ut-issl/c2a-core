/**
 * @file
 * @brief 受信した TC Frame の検証を行う
 */
#ifndef GS_VALIDATE_H_
#define GS_VALIDATE_H_

#include "../../TlmCmd/Ccsds/TCFrame.h"
#include "../../Library/stdint.h"

/**
 * @enum GS_VALIDATE_ERR
 * @brief gs_validate 関数の返り値
 */
typedef enum
{
  GS_VALIDATE_ERR_OK = 0,

  GS_VALIDATE_ERR_TCF_VER,
  GS_VALIDATE_ERR_TCF_SCID,
  GS_VALIDATE_ERR_TCF_VCID,
  GS_VALIDATE_ERR_TCF_TYPE,
  GS_VALIDATE_ERR_IN_LOCKOUT,
  GS_VALIDATE_ERR_TCS_SEQ_FLAG,
  GS_VALIDATE_ERR_TCP_VER,
  GS_VALIDATE_ERR_TCP_2ND_HDR_FLAG,
  GS_VALIDATE_ERR_APID,
  GS_VALIDATE_ERR_TCP_TYPE_IS_NOT_CMD,
  GS_VALIDATE_ERR_TCP_SEQ_IS_NOT_SINGLE,
  GS_VALIDATE_ERR_FECW_MISSMATCH,
  GS_VALIDATE_ERR_FARM1_POSITIVE_WINDOW_AREA,
  GS_VALIDATE_ERR_FARM1_NEGATIVE_WINDOW_AREA,
  GS_VALIDATE_ERR_FARM1_LOCKOUT_AREA,
  GS_VALIDATE_ERR_FARM1_SAME_NUMBER,
  GS_VALIDATE_ERR_INVALID_BC_CMD
} GS_VALIDATE_ERR;
// FIXME: TCP → SpacePacket 大工事後に TCP を直す

/**
 * @struct GS_ValidateInfo
 * @brief  直近に受信したパケットの検証結果を格納
 */
typedef struct
{
  // FARM-A,B用のカウンタ値
  uint8_t type_a_counter;
  uint8_t type_b_counter;
  // FARM-1用の情報
  uint8_t lockout_flag;
  uint8_t retransmit_flag;
  uint8_t positive_window_width;
} GS_ValiateInfo;

extern const GS_ValiateInfo* const gs_validate_info;

/**
 * @brief GS_ValiateInfo の初期化
 */
void GS_validate_init(void);

/**
 * @brief tc_frame の検証
 * @param[in] tc_frame: 検証する tc frame
 * @return GS_VALIDATE_ERR
 */
GS_VALIDATE_ERR GS_validate_tc_frame(const TCF* tc_frame);

/**
 * @brief テレメ用の clcw の作成
 * @return clcw
 */
uint32_t GS_form_clcw(void);

/**
 * @brief positive_window_width の設定
 * @param[in] positive_window_width
 * @return GS_VALIDATE_ERR
 */
GS_VALIDATE_ERR GS_set_farm_pw(uint8_t positive_window_width);

#endif
