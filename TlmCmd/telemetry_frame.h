/**
 * @file
 * @brief  テレメトリの登録，テレメトリ関連処理
 */
#ifndef TELEMETRY_FRAME_H_
#define TELEMETRY_FRAME_H_

#include "./common_tlm_cmd_packet.h"

#define TF_TLM_PAGE_SIZE (64)                                    //!< テレメテーブルの1テレメトリパケット(=1ページ)に格納されるテレメ数
#define TF_TLM_PAGE_MAX  (4)                                     //!< テレメテーブルページ数（ページネーション用）
#define TF_MAX_TLMS      (TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX)    //!< テレメテーブルサイズ．すなわち登録できる最大テレメ数

#include <src_user/Settings/TlmCmd/telemetry_frame_params.h>

/**
 * @struct TF_TlmInfo
 * @brief  tlm の情報
 * @note   tlm_func は，成功時はテレメ長，失敗時は TF_ACK を返す
 */
typedef struct
{
  int (*tlm_func)(unsigned char*, int);
} TF_TlmInfo;

/**
 * @enum   TF_ACK
 * @brief  tlm_func などの返り値につかう
 * @note   int8_t を想定（整数はテレメ長で使うので，負数）
 */
typedef enum
{
  TF_SUCCESS = 0,
  TF_TOO_SHORT_LEN = -1,
  TF_NOT_DEFINED = -2,
  TF_UNKNOWN = -3
} TF_ACK;

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
 * @brief  テレメを実際に生成する関数
 * @param  packet_id: Tlm ID
 * @param  contents:  テレメを作る body へのポインタ
 * @param  max_len:   テレメの body として使える最大長
 * @return 成功時はテレメ長，失敗時は TF_ACK
 */
int TF_generate_contents(int packet_id,
                         unsigned char* contents,
                         int max_len);

/**
 * @brief  Tlm Tableのロード
 * @note   定義は /src_user/TlmCmd/TelemetryDefinitions.c にある
 * @param  tlm_table: Tlm Tableの実体
 * @return void
 */
void TF_load_tlm_table(TF_TlmInfo tlm_table[TF_MAX_TLMS]);

CCP_EXEC_STS Cmd_TF_REGISTER_TLM(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_TF_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

void TF_copy_u8(uint8_t* ptr,
                uint8_t data);

void TF_copy_u16(uint8_t* ptr,
                 uint16_t data);

void TF_copy_u32(uint8_t* ptr,
                 uint32_t data);

void TF_copy_i8(uint8_t* ptr,
                int8_t data);

void TF_copy_i16(uint8_t* ptr,
                 int16_t data);

void TF_copy_i32(uint8_t* ptr,
                 int32_t data);

void TF_copy_float(uint8_t* ptr,
                   float data);

void TF_copy_double(uint8_t* ptr,
                    double data);

#endif
