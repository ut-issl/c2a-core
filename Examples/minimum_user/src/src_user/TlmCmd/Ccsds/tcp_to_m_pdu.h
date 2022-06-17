/**
 * @file
 * @brief PacketList から TCPacket を取ってきてそれを送信可能な M_PDU へと変換する
 */
#ifndef TCP_TO_M_PDU_H_
#define TCP_TO_M_PDU_H_

// FIXME: TCP から TlmSpacePacket に直す！！！

#include <stddef.h> // for size_t

#include <src_core/TlmCmd/packet_list.h>
#include "m_pdu.h"
#include <src_core/System/TimeManager/time_manager.h>

/**
 * @enum  T2M_ACK
 * @brief T2M 関数の返り値
 */
typedef enum
{
  T2M_SUCCESS,
  T2M_NO_DATA_TO_SEND,
  T2M_INVALID_M_PDU,
  TSM_UNKNOWN
} T2M_ACK;

/**
 * @struct TcpToMPdu
 * @brief  TCPacket を M_PDU に変換するときに必要なパラメータ
 */
typedef struct
{
  size_t tcp_rp;
  size_t m_pdu_wp;
  uint8_t fhp_valid;
  cycle_t flush_interval;
  cycle_t last_updated;
} TcpToMPdu;

/**
 * @brief T2M 構造体の初期化
 * @param[out] tcp_to_m_pdu: 初期化する T2M
 * @return void
 */
void T2M_initialize(TcpToMPdu* tcp_to_m_pdu);

/**
 * @brief TCPacket を M_PDU に変換する
 * @param[in] pl: TCPacket を取得する Packet List
 * @param[in] tcp_to_m_pdu: 変換する時のパラメータ群
 * @param[out] m_pdu: 生成される M_PDU
 * @return T2M_ACK
 */
T2M_ACK T2M_form_m_pdu(TcpToMPdu* tcp_to_m_pdu, PacketList* pl, M_PDU* m_pdu);

#endif
