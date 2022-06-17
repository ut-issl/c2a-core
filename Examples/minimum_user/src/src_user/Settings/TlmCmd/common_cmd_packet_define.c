#pragma section REPRO
/**
 * @file
 * @brief  CCP 型の定義
 */
#include "common_cmd_packet_define.h"

CCP_DEST_TYPE CCP_get_dest_type_from_uint8(uint8_t dest_type)
{
  switch ((CCP_DEST_TYPE)dest_type)
  {
  case CCP_DEST_TYPE_TO_ME:   // FALL THROUGH
  case CCP_DEST_TYPE_TO_MOBC: // FALL THROUGH
  case CCP_DEST_TYPE_TO_AOBC: // FALL THROUGH
  case CCP_DEST_TYPE_TO_TOBC:
    return (CCP_DEST_TYPE)dest_type;
  default:
    return CCP_DEST_TYPE_TO_UNKOWN;
  }
}

#pragma section
