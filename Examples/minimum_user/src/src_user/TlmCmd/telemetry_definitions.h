/**
 * @file
 * @brief  テレメトリ定義
 * @note   このコードは自動生成されています！
 */
#ifndef TELEMETRY_DEFINITIONS_H_
#define TELEMETRY_DEFINITIONS_H_

typedef enum
{
  Tlm_CODE_MOBC = 0x00,
  Tlm_CODE_MEM = 0x10,
  Tlm_CODE_TLM_MGR1 = 0x18,
  Tlm_CODE_TLM_MGR2 = 0x19,
  Tlm_CODE_TL = 0x20,
  Tlm_CODE_BL = 0x21,
  Tlm_CODE_CA = 0x25,
  Tlm_CODE_TF = 0x26,
  Tlm_CODE_DCU = 0x2a,
  Tlm_CODE_MM = 0x40,
  Tlm_CODE_AM = 0x45,
  Tlm_CODE_APP_TIME = 0x46,
  Tlm_CODE_EL = 0x56,
  Tlm_CODE_EL_TLOG = 0x57,
  Tlm_CODE_EL_CLOG = 0x58,
  Tlm_CODE_EH = 0x5a,
  Tlm_CODE_EH_RULE = 0x5b,
  Tlm_CODE_EH_LOG = 0x5c,
  Tlm_CODE_EH_INDEX = 0x5d,
  Tlm_CODE_GS = 0x78,
  Tlm_CODE_HK = 0xf0,
  Tlm_CODE_GIT_REV = 0xf1,
  Tlm_CODE_UART_TEST = 0xf8,

  TLM_CODE_MAX
} TLM_CODE;

#endif
