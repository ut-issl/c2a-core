/**
 * @file
 * @brief GS Driver のインスタンス化
 */
#ifndef DI_GS_H_
#define DI_GS_H_

#include "../../Drivers/Com/gs.h"
#include "../../TlmCmd/Ccsds/tcp_to_m_pdu.h"
#include "../../TlmCmd/Ccsds/vcdu.h"
#include "../../Library/stdint.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @struct DI_GS_TlmPacketHandler
 * @brief  テレメの送信
 */
typedef struct
{
  TcpToMPdu tc_packet_to_m_pdu; //!< PL から取り出した tc_packet を M_PDU に変換する
  VCDU vcdu;              //!< 送信するVCDU
  uint32_t vcdu_counter;  //!< VCDU counter
} DI_GS_TlmPacketHandler;

extern const GS_Driver* const gs_driver;
extern const DI_GS_TlmPacketHandler* const DI_GS_ms_tlm_packet_handler;
extern const DI_GS_TlmPacketHandler* const DI_GS_rp_tlm_packet_handler;

// 以下インスタンス化
AppInfo DI_GS_cmd_packet_handler(void);

AppInfo DI_GS_mst_packet_handler(void);
AppInfo DI_GS_rpt_packet_handler(void);

CCP_EXEC_STS Cmd_DI_GS_CCSDS_TX_START(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_CCSDS_TX_STOP(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_DRIVER_RESET(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_SET_MS_FLUSH_INTERVAL(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_SET_RP_FLUSH_INTERVAL(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_SET_FARM_PW(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_SET_INFO(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_GS_CCSDS_GET_BUFFER(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_GS_CCSDS_SET_RATE(const CommonCmdPacket* packet);

#endif
