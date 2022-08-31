#ifndef ANOMALY_HANDLER_H_
#define ANOMALY_HANDLER_H_

#include <src_user/Settings/System/anomaly_logger_define.h>
#ifdef AL_ENABLE

#include <stddef.h> // for size_t

#include "../System/AnomalyLogger/anomaly_logger.h"
#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"
#include <src_user/TlmCmd/block_command_definitions.h>
#include "../TlmCmd/block_command_table.h"
#include <src_user/Settings/System/anomaly_logger_group.h>
#include <src_user/Settings/Applications/AnomalyHandlerRules/ah_rules.h>

#define AH_TLM_PAGE_SIZE (32)                                  //!< AnomalyHandlerのルールテーブルの1テレメトリパケット(=1ページ)に格納されるルール数（ページネーション用）
#define AH_TLM_PAGE_MAX  (4)                                   //!< AnomalyHandlerのルールテーブルのページ数（ページネーション用）
#define AH_MAX_RULES (AH_TLM_PAGE_SIZE * AH_TLM_PAGE_MAX)        //!< 最大何個のルール（アノマリ - イベント対応）を保持できるか

#define AH_LOG_TLM_PAGE_SIZE (32)                              //!< AnomalyHandlerの発動イベントログテーブルの1テレメトリパケット(=1ページ)に格納される発動イベントログ数（ページネーション用）
#define AH_LOG_TLM_PAGE_MAX  (4)                               //!< AnomalyHandlerの発動イベントログテーブルのページ数（ページネーション用）
#define AH_LOG_MAX (AH_LOG_TLM_PAGE_SIZE * AH_LOG_TLM_PAGE_MAX)  //!< 最大何個のイベントログを保持できるか
// 2018-12-11
// ↑これって， AL_RECORD_MAX とサイズ揃える必要ないの？
// なさそう？

#include <src_user/Settings/Applications/anomaly_handler_params.h>

typedef enum
{
  AH_SINGLE,
  AH_CONTINUOUS,
  AH_CUMULATE
} AH_CONDITION;

typedef struct
{
  AL_AnomalyCode  code;
  AH_CONDITION cond;
  size_t       threshold;     // tlmではunit8_tだ....    AH_CUMULATE用のカウンタ
  bct_id_t     bc_id;         // tlmではunit8_tだ....    // TODO 2019/08/30 BCTの形が変わったので，TLMも直す！！！
} AH_Rule;

typedef struct
{
  int     is_active;
  AH_Rule rule;
  size_t  counter;            // AH_CUMULATE用のカウンタ
} AH_Element;

typedef struct
{
  size_t count;
  size_t pos;
} AH_Pointer;

typedef struct
{
  AH_Element elements[AH_MAX_RULES];
  AH_Pointer al_pointer;
  ObcTime    respond_at;
  size_t     latest_id;             // tlmではunit8_tだ....
  size_t     action_counter;        // tlmではunit8_tだ....
  size_t     latest_run_length;     // tlmではunit8_tだ....
  uint8_t    page_no;               // tlmではunit8_tだ....
} AnomalyHandler;

typedef struct
{
  size_t respond_at_master;
  size_t rule_num;
} AH_AnomalyRespond;

typedef struct
{
  AH_AnomalyRespond log[AH_LOG_MAX];
  size_t            pointer;
  uint8_t           page_no;
} AH_AnomalyRespondLog;


extern const AnomalyHandler* const anomaly_handler;
extern const AH_AnomalyRespondLog* const AH_respond_log;

AppInfo AH_create_app(void);

AppInfo print_ah_status(void);

void AH_activate_rule(size_t id);

void AH_inactivate_rule(size_t id);

// staticな方はデフォルトでは無効だが，
// こいつは初期化時に使われるので，選択制にした
void AH_add_rule(size_t id, const AH_Rule* ahr, uint8_t is_active);

CCP_CmdRet Cmd_AH_REGISTER_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AH_ACTIVATE_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AH_INACTIVATE_RULE(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AH_CLEAR_LOG(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AH_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AHRES_LOG_CLEAR(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_AHRES_LOG_SET_PAGE_FOR_TLM(const CommonCmdPacket* packet);

#endif
#endif
