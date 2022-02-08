#pragma section REPRO
/**
* @file
* @brief  AOBC の DI と AOBC の cmd dispatcher
* @note   C2A 間通信のデモ用であり， AOBC も C2A 搭載を仮定する
*/

#include "di_aobc.h"
#include "../../Drivers/Aocs/aobc.h"
#include "../../TlmCmd/user_packet_handler.h"
#include <src_core/Library/print.h>
#include "../../Settings/port_config.h"


static AOBC_Driver aobc_driver_;
const AOBC_Driver* const aobc_driver = &aobc_driver_;

static CommandDispatcher DI_AOBC_cdis_;
const CommandDispatcher* const DI_AOBC_cdis = &DI_AOBC_cdis_;

static void DI_AOBC_init_(void);
static void DI_AOBC_update_(void);

static void DI_AOBC_cmd_dispatcher_init_(void);
static void DI_AOBC_cmd_dispatcher_(void);


AppInfo DI_AOBC_update(void)
{
  return AI_create_app_info("update_AOBC", DI_AOBC_init_, DI_AOBC_update_);
}


static void DI_AOBC_init_(void)
{
  int ret = AOBC_init(&aobc_driver_, PORT_CH_RS422_AOBC);

  if (ret != 0)
  {
    Printf("AOBC init Failed ! %d \n", ret);
  }
}


static void DI_AOBC_update_(void)
{
  int ret;
  ret = AOBC_rec(&aobc_driver_);

  // [TODO]
  // 必要があればここに処理を
  (void)ret;
}


AppInfo DI_AOBC_cmd_dispatcher(void)
{
  return AI_create_app_info("aobc_cdis",
                            DI_AOBC_cmd_dispatcher_init_,
                            DI_AOBC_cmd_dispatcher_);
}


static void DI_AOBC_cmd_dispatcher_init_(void)
{
  DI_AOBC_cdis_ = CDIS_init(&PH_aobc_cmd_list);
}


static void DI_AOBC_cmd_dispatcher_(void)
{
  CDIS_dispatch_command(&DI_AOBC_cdis_);
}


CCP_EXEC_STS DI_AOBC_dispatch_command(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  CommonCmdPacket* pckt = (CommonCmdPacket*)packet; // const_cast
  // ここで CCP_DEST_TYPE を宛先で受理できるように変更（なので const cast が発生している．．．）

  if (CCP_get_dest_type(pckt) != CCP_DEST_TYPE_TO_ME)
  {
    // MOBC のキューに入らず直接転送
    // そのままの EXEC_TYPE で転送．なにもしない
  }
  else
  {
    // MOBC のキューに溜まった後に実行されたもの
    // 配送先 OBC では MOBC 側の TL などの影響は受けないはずなので RTC へ変換
    CCP_set_exec_type(pckt, CCP_EXEC_TYPE_RT);
  }

  // 配送先 OBC が最終到達地なので
  CCP_set_dest_type(pckt, CCP_DEST_TYPE_TO_ME);

  ret = AOBC_send_cmd(&aobc_driver_, pckt);
  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}


CCP_EXEC_STS Cmd_DI_AOBC_CDIS_CLEAR_ALL_REALTIME(const CommonCmdPacket* packet)
{
  (void)packet;

  CDIS_clear_command_list(&DI_AOBC_cdis_);
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_DI_AOBC_CDIS_CLEAR_ERR_LOG(const CommonCmdPacket* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除
  CDIS_clear_error_status(&DI_AOBC_cdis_);
  return CCP_EXEC_SUCCESS;
}

#pragma section
