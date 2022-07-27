#pragma section REPRO
#include "sequence_items.h"
#include "../../../TlmCmd/block_command_definitions.h"
#include <src_core/Library/endian_memcpy.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../Settings/System/anomaly_logger_group.h"
#include "../mode_definitions.h"


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// SIを使うことはもう非推奨！！！
// 普通のBCを使うこと！！！！
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void SI_finish_transition(CommonCmdPacket* packet)
{
  CCP_form_rtc(packet, Cmd_CODE_MM_FINISH_TRANSITION, NULL, 0);
}

void SI_start_hk_tlm(CommonCmdPacket* packet)
{
  CCP_form_block_deploy_cmd(packet, TLCD_ID_DEPLOY_TLM, BC_HK_CYCLIC_TLM);
}

// BC展開
void SI_deploy_block(CommonCmdPacket* packet, TLCD_ID line_no, bct_id_t block_no)
{
  CCP_form_block_deploy_cmd(packet, line_no, block_no);
}

#pragma section
