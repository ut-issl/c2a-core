#pragma section REPRO
// 地上局からのコマンド実行用アプリ

#include "gs_command_dispatcher.h"

#include "../CmdTlm/packet_handler.h"

static CommandDispatcher gs_command_dispatcher_;
const CommandDispatcher* const gs_command_dispatcher = &gs_command_dispatcher_;

static void GSCD_init_(void);
static void GSCD_dispatch_(void);

AppInfo GSCD_create_app(void)
{
  return AI_create_app_info("gs_command_dispatcher", GSCD_init_, GSCD_dispatch_);
}

static void GSCD_init_(void)
{
  gs_command_dispatcher_ = CDIS_init(&PH_gs_cmd_list);
}

static void GSCD_dispatch_(void)
{
  // このgs_command_dispatcher_は外部で更新されている
  // 2018/06/26 の時点における処理の流れを具体的に描くと、
  // GSTOS_cmd_packet_handler_ > PH_analyze_packet > analyze_cmd_ > add_gs_cmd_
  //
  // AR_DI_GSTOS_CMD_PH というIDのアプリを起点にコマンドの解析が進み、
  // 地上局からのコマンドと判定されるとadd_gs_cmd_関数内でgs_command_dispatcher_に反映される。

  CDIS_dispatch_command(&gs_command_dispatcher_);
}

CCP_EXEC_STS Cmd_GSCD_CLEAR_ERR_LOG(const CTCP* packet)
{
  (void)packet;

  // 記録されたエラー情報を解除。
  CDIS_clear_error_status(&gs_command_dispatcher_);
  return CCP_EXEC_SUCCESS;
}
#pragma section
