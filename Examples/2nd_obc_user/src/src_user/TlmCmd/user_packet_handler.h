/**
 * @file
 * @brief  packet_handlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#ifndef USER_PACKET_HANDLER_H_
#define USER_PACKET_HANDLER_H_

#include <src_core/TlmCmd/packet_list.h>
#include <src_core/TlmCmd/common_tlm_cmd_packet.h>
#include <src_core/TlmCmd/packet_handler.h>

/**
 * @brief  PH のユーザー固有部初期化処理
 *
 *         PacketList の初期化を行う
 * @param  void
 * @return void
 */
void PH_user_init(void);

/**
 * @brief  PH の analyze_cmd_ のユーザー処理関数
 *
 *         Core の処理より優先される
 *         C2A を搭載したコンポのキュー（GSC，RTC，TLC，BC）に即座に直接追加される．
 *         正確には， add_rt_cmd_ の送信先版を作り，そこにキューイングされ，DI (dispatcher) によって回収される．
 *         つまり，本 OBC にとっては RTC だが，転送対象 OBC にとっては各種 Cmd 扱いな Cmd を転送する処理
 * @param  packet: CCP
 * @retval PH_ACK_FORWARDED: 無事に転送するキューに追加された
 * @retval PH_ACK_PL_LIST_FULL: PL LIST FULL
 * @retval PH_ACK_UNKNOWN: ユーザー処理に引っかからなかった場合．Core 側の処理に回される
 */
PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet);

/**
 * @brief  PH の cmd_router_ のユーザー処理関数
 *
 *         C2A を搭載したコンポに Cmd が転送される．
 *         つまり，転送対象 OBC にとっては RTC 扱いになる
 *         ここから Driver を叩いて送信まで行うことになる（実行時間は cmdExec と同じだけ許容されているので OK）
 * @param  packet: CCP
 * @retval CCP_EXEC_SUCCESS など:   無事に転送された．転送先の結果を返す
 * @retval CCP_EXEC_ROUTING_FAILED: 転送失敗（詳細エラーは DriverSuper を参照）
 */
CCP_EXEC_STS PH_user_cmd_router(const CommonCmdPacket* packet);

#endif
