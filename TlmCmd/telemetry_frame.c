#pragma section REPRO
/**
 * @file
 * @brief  テレメトリの登録，テレメトリ関連処理
 */
#include "telemetry_frame.h"

#include <stdint.h>
#include "../Library/print.h"
#include "../Library/endian.h"
#include "./common_cmd_packet_util.h"

static void initialize_tlm_table_(void);

static TelemetryFrame telemetry_frame_;
const TelemetryFrame* const telemetry_frame = &telemetry_frame_;


TF_TLM_FUNC_ACK TF_generate_contents(TLM_CODE tlm_id,
                                     uint8_t* packet,
                                     uint16_t* len,
                                     uint16_t max_len)
{
  TF_TLM_FUNC_ACK (*tlm_func)(uint8_t*, uint16_t*, uint16_t) = telemetry_frame->tlm_table[tlm_id].tlm_func;

  if (tlm_func != NULL)
  {
    return tlm_func(packet, len, max_len);
  }
  else
  {
    return TF_TLM_FUNC_ACK_NOT_DEFINED;
  }
}

void TF_initialize(void)
{
  // tlm_table_を初期化
  initialize_tlm_table_();

  if (TF_MAX_TLMS <= TLM_CODE_MAX)
  {
    Printf("TF: init error!!!\n");
    return;
  }

  TF_load_tlm_table(telemetry_frame_.tlm_table);
}

static void initialize_tlm_table_(void)
{
  int i;
  for (i = 0; i < TF_MAX_TLMS; ++i)
  {
    telemetry_frame_.tlm_table[i].tlm_func = NULL;
  }

  // ページ番号の初期値は0に設定
  telemetry_frame_.tlm_page_no = 0;
}

void TF_copy_u8(uint8_t* ptr, uint8_t data)
{
  ptr[0] = data;
}

void TF_copy_u16(uint8_t* ptr, uint16_t data)
{
  ENDIAN_memcpy(ptr, &data, 2);
}

void TF_copy_u32(uint8_t* ptr, uint32_t data)
{
  ENDIAN_memcpy(ptr, &data, 4);
}

void TF_copy_i8(uint8_t* ptr, int8_t data)
{
  ptr[0] = (uint8_t)data;
}

void TF_copy_i16(uint8_t* ptr, int16_t data)
{
  ENDIAN_memcpy(ptr, &data, 2);
}

void TF_copy_i32(uint8_t* ptr, int32_t data)
{
  ENDIAN_memcpy(ptr, &data, 4);
}

void TF_copy_float(uint8_t* ptr, float data)
{
  ENDIAN_memcpy(ptr, &data, sizeof(float));
}

void TF_copy_double(uint8_t* ptr, double data)
{
  ENDIAN_memcpy(ptr, &data, sizeof(double));
}

CCP_CmdRet Cmd_TF_INIT(const CommonCmdPacket* packet)
{
  (void)packet;
  TF_initialize();
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_TF_REGISTER_TLM(const CommonCmdPacket* packet)
{
  TLM_CODE tlm_id = (TLM_CODE)CCP_get_param_from_packet(packet, 0, uint8_t);
  uint32_t tlm_func = CCP_get_param_from_packet(packet, 1, uint32_t);

  if (tlm_id >= TF_MAX_TLMS)
  {
    // 登録指定位置がテレメトリ数上限を超えている場合は異常判定
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  telemetry_frame_.tlm_table[tlm_id].tlm_func = (TF_TLM_FUNC_ACK (*)(uint8_t*, uint16_t*, uint16_t))tlm_func;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_TF_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet)
{
  uint8_t page = CCP_get_param_from_packet(packet, 0, uint8_t);

  if (page >= TF_TLM_PAGE_MAX)
  {
    // ページ番号がコマンドテーブル範囲外
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  telemetry_frame_.tlm_page_no = page;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
