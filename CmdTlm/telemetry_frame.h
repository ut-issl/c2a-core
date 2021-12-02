/**
 * @file telemetry_frame.h
 * @brief  テレメトリの登録，テレメトリ関連処理
 */
#ifndef TELEMETRY_FRAME_H_
#define TELEMETRY_FRAME_H_

#include "./common_tlm_cmd_packet.h"

#define TF_TLM_PAGE_SIZE (64)                                    //!< テレメテーブルの1テレメトリパケット(=1ページ)に格納されるテレメ数
#define TF_TLM_PAGE_MAX  (4)                                     //!< テレメテーブルページ数（ページネーション用）
#define TF_MAX_TLMS      (TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX)    //!< テレメテーブルサイズ．すなわち登録できる最大テレメ数

#include <src_user/Settings/CmdTlm/telemetry_frame_params.h>

typedef struct
{
  int (*tlm_func)(unsigned char*, int);
} TF_TlmInfo;

typedef enum
{
  TLM_SUCCESS = 0,
  TLM_TOO_SHORT_LEN = -1,
  TLM_NOT_DEFINED = -2,
  TLM_UNKNOWN = -3
} TLM_ACK;

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


void TF_initialize(void);

int TF_generate_contents(int packet_id,
                         unsigned char* contents,
                         int max_len);

/**
 * @brief  Tlm Tableのロード
 * @note   定義は /src_user/CmdTlm/TelemetryDefinitions.c にある
 * @param  tlm_table: Tlm Tableの実体
 * @return void
 */
void TF_load_tlm_table(TF_TlmInfo tlm_table[TF_MAX_TLMS]);

CCP_EXEC_STS Cmd_TF_REGISTER_TLM(const CTCP* packet);

CCP_EXEC_STS Cmd_TF_SET_PAGE_FOR_TLM(const CTCP* packet);

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
