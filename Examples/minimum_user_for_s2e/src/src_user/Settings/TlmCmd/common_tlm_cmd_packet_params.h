/**
 * @file
 * @brief  CTCP パラメタのオーバーライド用ヘッダー
 */
#ifndef COMMON_TLM_CMD_PACKET_PARAMS_H_
#define COMMON_TLM_CMD_PACKET_PARAMS_H_

#include "../../TlmCmd/Ccsds/TCPacket.h"

#undef CTCP_MY_DST_ID

#define CTCP_MY_DST_ID (TCP_APID_MOBC_CMD)      //!< 自分宛てCMDを示すDestination IDを定義

#endif
