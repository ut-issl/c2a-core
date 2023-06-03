#pragma section REPRO
/**
 * @file
 * @brief  packet_handlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#include "user_packet_handler.h"
#include <src_core/TlmCmd/packet_list_util.h>


void PH_user_init(void)
{
}


PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet)
{
  switch (CCP_get_dest_type(packet))
  {
  default:
    // 2nd OBC なので，自分宛て以外のパケットはないはず
    return PH_ACK_UNKNOWN;
  }
}


CCP_CmdRet PH_user_cmd_router(const CommonCmdPacket* packet)
{
  APID apid = CCP_get_apid(packet);
  switch (apid)
  {
  default:
    // 該当する配送先が定義されていない場合
    return CCP_make_cmd_ret(CCP_EXEC_ROUTING_FAILED, apid);
  }
}


TF_TLM_FUNC_ACK PH_user_telemetry_router(APID apid,
                                         TLM_CODE tlm_id,
                                         uint8_t* packet,
                                         uint16_t* len,
                                         uint16_t max_len)
{
  (void)tlm_id;
  (void)packet;
  (void)len;
  (void)max_len;

  switch (apid)
  {
    default:
      return TF_TLM_FUNC_ACK_NOT_DEFINED;
  }
}

#pragma section
