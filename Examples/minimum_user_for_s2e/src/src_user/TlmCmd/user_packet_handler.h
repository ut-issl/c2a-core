/**
 * @file
 * @brief  packet_handlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#ifndef USER_PACKET_HANDLER_H_
#define USER_PACKET_HANDLER_H_

#include <src_core/CmdTlm/packet_handler.h>
#include <src_core/CmdTlm/common_tlm_cmd_packet.h>

extern PacketList PH_aobc_cmd_list;
extern PacketList PH_tobc_cmd_list;

/**
 * @brief  PHのユーザー固有部初期化処理
 *
 *         PacketListの初期化を行う
 * @param  void
 * @return void
 */
void PH_user_init(void);

/**
 * @brief  PHのanalyze_cmd_のユーザー処理関数
 *
 *         Coreの処理より優先される
 *         C2Aを搭載したコンポのキュー（GSC，RTC，TLC，BC）に即座に直接追加される．
 *         正確には，add_rt_cmd_の送信先版を作り，そこにキューイングされ，DI (dispatcher) によって回収される．
 *         つまり，本OBCにとってはRTCだが，転送対象OBCにとっては各種Cmd扱いなCmdを転送する処理
 * @param  *packet CTCP packet
 * @retval PH_FORWARDED: 無事に転送するキューに追加された
 * @retval PH_PL_LIST_FULL: PL LIST FULL
 * @retval PH_FORWARDED以外（PH_UNKNOWNを想定）: ユーザー処理に引っかからなかった場合．Core側の処理に回される
 */
PH_ACK PH_user_analyze_cmd(const CTCP* packet);

/**
 * @brief  PHのcmd_router_のユーザー処理関数
 *
 *         C2Aを搭載したコンポにCmdが転送される．
 *         つまり，転送対象OBCにとってはRTC扱いになる
 *         ここからDriverを叩いて送信まで行うことになる（実行時間はcmdExecと同じだけ許容されているのでOK）
 * @param  *packet CTCP packet
 * @retval CCP_EXEC_SUCCESS など:   無事に転送された．転送先の結果を返す
 * @retval CCP_EXEC_ROUTING_FAILED: 転送失敗（詳細エラーはDriverSuperを参照）
 */
CCP_EXEC_STS PH_user_cmd_router(const CTCP* packet);

#endif
