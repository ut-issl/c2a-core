/**
 * @file
 * @brief  テレメトリの登録，テレメトリ関連処理
 */
#ifndef TELEMETRY_FRAME_H_
#define TELEMETRY_FRAME_H_

#include "./common_cmd_packet.h"
#include <src_user/TlmCmd/telemetry_definitions.h>

#define TF_TLM_PAGE_SIZE (64)                                    //!< テレメテーブルの1テレメトリパケット(=1ページ)に格納されるテレメ数
#define TF_TLM_PAGE_MAX  (4)                                     //!< テレメテーブルページ数（ページネーション用）
#define TF_MAX_TLMS      (TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX)    //!< テレメテーブルサイズ．すなわち登録できる最大テレメ数

#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>

/**
 * @enum   TF_TLM_FUNC_ACK
 * @brief  tlm_func の返り値につかう
 * @note   uint8_t を想定
 */
typedef enum
{
  TF_TLM_FUNC_ACK_SUCCESS,        //!< 成功
  TF_TLM_FUNC_ACK_TOO_SHORT_LEN,  //!< CommonTlmPacket が，生成されるテレメに比べて小さすぎる（定義されたテレメが大きすぎる）
  TF_TLM_FUNC_ACK_NOT_DEFINED,    //!< 定義されてないテレメ
  TF_TLM_FUNC_ACK_NULL_PACKET,    //!< 2nd OBC などのテレメを生成する時，まだ１つもそのテレメパケットを 2nd OBC から受信していない
  TF_TLM_FUNC_ACK_UNKNOWN_ERR     //!< その他のエラー
} TF_TLM_FUNC_ACK;

/**
 * @struct TF_TlmInfo
 * @brief  tlm の情報
 */
typedef struct
{
  TF_TLM_FUNC_ACK (*tlm_func)(uint8_t*, uint16_t*, uint16_t);   //!< tlm packet の中身を生成する関数
} TF_TlmInfo;

/**
 * @struct TelemetryFrame
 * @brief  TelemetryFrame の Info 構造体
 */
typedef struct
{
  TF_TlmInfo tlm_table[TF_MAX_TLMS];  //!< テレメトリテーブル
  uint8_t tlm_page_no;                //!< テレメで使うページ数
} TelemetryFrame;

extern const TelemetryFrame* const telemetry_frame;


/**
 * @brief  TF の初期化
 * @param  void
 * @return void
 */
void TF_initialize(void);

/**
 * @brief テレメを実際に生成する関数
 * @param[in]  tlm_id:  Tlm ID
 * @param[out] packet:  テレメを作る uint8_t にシリアライズされた packet へのポインタ
 * @param[out] len:     生成したテレメのパケット長
 * @param[in]  max_len: 許容できる最大テレメパケット長
 * @return TF_TLM_FUNC_ACK
 */
TF_TLM_FUNC_ACK TF_generate_contents(TLM_CODE tlm_id,
                                     uint8_t* packet,
                                     uint16_t* len,
                                     uint16_t max_len);

/**
 * @brief  Tlm Tableのロード
 * @note   定義は /src_user/TlmCmd/TelemetryDefinitions.c にある
 * @param  tlm_table: Tlm Tableの実体
 * @return void
 */
void TF_load_tlm_table(TF_TlmInfo tlm_table[TF_MAX_TLMS]);

CCP_EXEC_STS Cmd_TF_INIT(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_TF_REGISTER_TLM(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_TF_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

void TF_copy_u8(uint8_t* ptr, uint8_t data);

void TF_copy_u16(uint8_t* ptr, uint16_t data);

void TF_copy_u32(uint8_t* ptr, uint32_t data);

void TF_copy_i8(uint8_t* ptr, int8_t data);

void TF_copy_i16(uint8_t* ptr, int16_t data);

void TF_copy_i32(uint8_t* ptr, int32_t data);

void TF_copy_float(uint8_t* ptr, float data);

void TF_copy_double(uint8_t* ptr, double data);

#endif
