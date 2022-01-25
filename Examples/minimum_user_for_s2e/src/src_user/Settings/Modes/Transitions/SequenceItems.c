#pragma section REPRO
#include "SequenceItems.h"
#include "../../../TlmCmd/block_command_definitions.h"
#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/Library/endian_memcpy.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../Settings/AnomalyLogger/anomaly_group.h"
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
  unsigned char param[1 + SIZE_OF_BCT_ID_T];
  bct_id_t bc_id;

  param[0] = TL_ID_DEPLOY_TLM; // TL2に展開
  bc_id = BC_HK_CYCLIC_TLM;
  endian_memcpy(&param[1], &bc_id, SIZE_OF_BCT_ID_T);
  CCP_form_rtc(packet, Cmd_CODE_TLCD_DEPLOY_BLOCK, &param[0], 1 + SIZE_OF_BCT_ID_T);
}


// BC展開
void SI_deploy_block(CommonCmdPacket* packet, int line_no, bct_id_t block_no)
{
  unsigned char param[1 + SIZE_OF_BCT_ID_T];

  param[0] = (unsigned char)line_no;
  endian_memcpy(&param[1], &block_no, SIZE_OF_BCT_ID_T);
  CCP_form_rtc(packet, Cmd_CODE_TLCD_DEPLOY_BLOCK, &param[0], 1 + SIZE_OF_BCT_ID_T);
}

#pragma section
