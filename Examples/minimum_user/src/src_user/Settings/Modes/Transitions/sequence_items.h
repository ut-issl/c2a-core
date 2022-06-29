#ifndef SEQUENCE_ITEMS_H_
#define SEQUENCE_ITEMS_H_

#include <src_core/TlmCmd/block_command_table.h>
#include <src_core/Applications/timeline_command_dispatcher.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// SIを使うことはもう非推奨！！！
// 普通のBCを使うこと！！！！
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void SI_finish_transition(CommonCmdPacket* packet);

void SI_start_hk_tlm(CommonCmdPacket* packet);

// BC展開
void SI_deploy_block(CommonCmdPacket* packet, TLCD_ID line_no, bct_id_t block_no);

#endif
