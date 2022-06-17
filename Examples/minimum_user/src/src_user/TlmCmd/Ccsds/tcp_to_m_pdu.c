#pragma section REPRO
/**
 * @file
 * @brief PacketList から TCPacket を取ってきてそれを送信可能な M_PDU へと変換する
 */

#include "tcp_to_m_pdu.h"
// FIXME: CTP ではなく TSP を使ってしまっている．できれば抽象化するべき
#include <src_core/TlmCmd/Ccsds/tlm_space_packet.h>

void T2M_initialize(TcpToMPdu* tcp_to_m_pdu)
{
  // TC Packet Read Pointerの値を初期化
  tcp_to_m_pdu->tcp_rp = 0;
  tcp_to_m_pdu->m_pdu_wp = 0;
  tcp_to_m_pdu->fhp_valid = 0;
  // 強制送出待ち時間の初期値は10秒
  // 32kbpsなら8VCDU/secの送信能力
  tcp_to_m_pdu->flush_interval = OBCT_sec2cycle(10);
  // 最終更新時刻は現在時刻に設定
  tcp_to_m_pdu->last_updated = TMGR_get_master_total_cycle();

  return;
}

T2M_ACK T2M_form_m_pdu(TcpToMPdu* tcp_to_m_pdu, PacketList* pl, M_PDU* m_pdu)
{
  // M_PDUが完成する or TC Packetがなくなるまで実施
  while (tcp_to_m_pdu->m_pdu_wp != M_PDU_DATA_SIZE)
  {
    const TlmSpacePacket* packet;
    size_t tcp_len, tcp_left, m_pdu_left, write_len;

    if (PL_is_empty(pl))
    {
      // テレメトリQueueに送出すべきパケットがない場合
      // 最終更新時刻からの経過時間を算出
      cycle_t delta = TMGR_get_master_total_cycle() - tcp_to_m_pdu->last_updated;

      if (tcp_to_m_pdu->m_pdu_wp == 0)
      {
        // M_PDUのWrite Pointerが0で送出データがない場合
        // 送出データなしとして処理打ち切り
        return T2M_NO_DATA_TO_SEND;
      }

      if (delta < tcp_to_m_pdu->flush_interval)
      {
        // 経過時間が強制送出間隔に達していない場合
        // 処理を打ち切り、M_PDU未完成として0を返す
        return T2M_INVALID_M_PDU;
      }
      else
      {
        // 強制送出のために埋めるべきデータ量を計算し、
        // Fill Packetを生成、Queueに追加。
        //
        // M_PDUの残り領域がFill Packetのヘッダ長以下の場合、
        // ユーザデータ長1のFill Packetが生成される。
        // この場合、生成されたFill Packetは次M_PDUにまたがる。
        // この状態で追加のテレメトリが生成されない場合は、Fill
        // Packetのみで構成されたM_PDUが一度送出されることになる。
        static TlmSpacePacket fill_; // サイズが大きいため静的確保(スタック保護)
        size_t fill_size = M_PDU_DATA_SIZE - tcp_to_m_pdu->m_pdu_wp;
        TSP_setup_fill_packet(&fill_, (uint16_t)fill_size);
        PL_push_back(pl, &fill_);
      }
    }

    // Queue先頭のTC Packetを取得
    // 有効パケットまたはFillパケットが必ず入っている。
    packet = (const TlmSpacePacket*)(PL_get_head(pl)->packet);   // FIXME: Space Packet 実装でなおす

    // 書き込むデータ長を計算
    tcp_len = TSP_get_packet_len(packet);
    tcp_left = tcp_len - tcp_to_m_pdu->tcp_rp;
    m_pdu_left = M_PDU_DATA_SIZE - tcp_to_m_pdu->m_pdu_wp;
    write_len = (tcp_left > m_pdu_left) ? m_pdu_left : tcp_left;

    // First Header Pointerが未設定かつ書き込み開始がパケット先頭
    if ((tcp_to_m_pdu->fhp_valid != 1) && (tcp_to_m_pdu->tcp_rp == 0))
    {
      // First Header Pointerを現在のWrite Pointerの値に設定
      M_PDU_set_1st_hdr_ptr(m_pdu, (uint16_t)tcp_to_m_pdu->m_pdu_wp);
      // First Header Pointer設定済みを記録
      tcp_to_m_pdu->fhp_valid = 1;
    }

    // M_PDUデータの末尾にデータを追加
    M_PDU_set_data(m_pdu,
                   &(packet->packet[tcp_to_m_pdu->tcp_rp]),
                   tcp_to_m_pdu->m_pdu_wp,
                   write_len);

    // TC Pacekt Read Pointerを更新
    tcp_to_m_pdu->tcp_rp += write_len;
    // M_PDU Write Pointerを更新
    tcp_to_m_pdu->m_pdu_wp += write_len;
    // 最終書き込み時刻を更新
    tcp_to_m_pdu->last_updated = TMGR_get_master_total_cycle();

    // TC Packet全体をM_PDUに書き込み終わった場合
    if (tcp_to_m_pdu->tcp_rp == tcp_len)
    {
      // 書き込み完了したTC PacketをQueueから破棄しRead Poineterの値を初期化
      PL_drop_executed(pl);
      tcp_to_m_pdu->tcp_rp = 0;
    }
  }

  // M_PDUを埋め終わったのにFirst Header Pointerが未設定の場合
  // -> パケット先頭がM_PDUの中に含まれなかった場合
  if (tcp_to_m_pdu->fhp_valid != 1)
  {
    // First Header Pointerを「ヘッダなし」に設定
    M_PDU_set_1st_hdr_ptr(m_pdu, M_PDU_PTR_NO_HDR);
    // First Header Pointer設定済みを記録
    tcp_to_m_pdu->fhp_valid = 1;
  }

  // M_PDU Write Pointerの値をクリア
  tcp_to_m_pdu->m_pdu_wp = 0;
  // First Header Pointer状態を未設定に変更
  tcp_to_m_pdu->fhp_valid = 0;

  return T2M_SUCCESS;
}

#pragma section
