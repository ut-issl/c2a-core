#pragma section REPRO
/**
 * @file
 * @brief  ブロックコマンド登録関連のユーティリティ
 *
 *         本機能はブロックコマンドの登録部をwrapし，userが簡潔かつ安全な形でブロック
 *         コマンド登録を行えるようにするためのユーティリティ機能です．
 */

#include <string.h>
#include "block_command_loader.h"
#include "../Applications/timeline_command_dispatcher_id_define.h"
#include "block_command_executor.h"
#include <src_user/TlmCmd/command_definitions.h> // for rotate/combine block
#include "common_cmd_packet_util.h"
#include "../Library/endian.h"

#define BCL_PARAM_MAX_LENGTH BCT_CMD_MAX_LENGTH

static void BCL_register_cmd_(cycle_t ti, CMD_CODE cmd_id);
static void BCL_register_cmd_to_other_obc_(cycle_t ti, APID apid, CMD_CODE cmd_id);
static void BCL_register_app_(cycle_t ti, AR_APP_ID app_id);
static void BCL_clear_info_(void);


/**
 * @brief  ブロックコマンド登録中の情報を一時保存するためのバッファ
 */
typedef struct
{
  CommonCmdPacket packet;                     //!< 登録中のコマンドを一時保存するためのバッファ
  uint8_t params[BCL_PARAM_MAX_LENGTH];       /*!< 登録中のパラメータを一時保存するためのバッファ
                                                   初期化を楽にするために一コマンド分静的に確保している
                                                   TODO: サイズが過剰かもしれないので時間あれば調整 */
  int param_idx;                              //!< params配列の中で次にパラメータを登録する予定のindex
} BlockCommandLoader;

static BlockCommandLoader block_command_loader_;


void BCL_load_bc(bct_id_t pos, void (*BCL_load_func)(void))
{
  if (pos >= BCT_MAX_BLOCKS) return;

  // 最初にゴミデータを消しておく
  BCL_clear_info_();

  BCT_store_pos();
  BCT_clear_block(pos);

  BCL_load_func();

  // TODO: load内容の検証を入れる
  //       実行時エラーなので，多少変なのはとりあえず起動させてアノマリで確認か？

  BCE_activate_block();
  BCT_restore_pos();
}

#if 0
void BCL_safe_load_tl(bct_id_t pos, void (*BCL_load_func)(void))
{
  // pass
}

void BCL_safe_load_sl(bct_id_t pos, void (*BCL_load_func)(void))
{
  // pass
}
#endif

void BCL_tool_register_cmd(cycle_t ti, CMD_CODE cmd_id)
{
  BCL_register_cmd_(ti, cmd_id);
  BCL_clear_info_();
}

void BCL_tool_register_cmd_to_other_obc(cycle_t ti, APID apid, CMD_CODE cmd_id)
{
  BCL_register_cmd_to_other_obc_(ti, apid, cmd_id);
  BCL_clear_info_();
}

void BCL_tool_register_rotate(cycle_t ti, bct_id_t bct_id)
{
#if SIZE_OF_BCT_ID_T == 1
  BCL_tool_prepare_param_uint8(bct_id);
#elif SIZE_OF_BCT_ID_T == 2
  BCL_tool_prepare_param_uint16(bct_id);
#elif SIZE_OF_BCT_ID_T == 4
  BCL_tool_prepare_param_uint32(bct_id);
#else
#error Illegal value for SIZE_OF_BCT_ID_T
#endif

  BCL_register_cmd_(ti, Cmd_CODE_BCE_ROTATE_BLOCK);
  BCL_clear_info_();
}

void BCL_tool_register_combine(cycle_t ti, bct_id_t bct_id)
{
#if SIZE_OF_BCT_ID_T == 1
  BCL_tool_prepare_param_uint8(bct_id);
#elif SIZE_OF_BCT_ID_T == 2
  BCL_tool_prepare_param_uint16(bct_id);
#elif SIZE_OF_BCT_ID_T == 4
  BCL_tool_prepare_param_uint32(bct_id);
#else
#error Illegal value for SIZE_OF_BCT_ID_T
#endif

  BCL_register_cmd_(ti, Cmd_CODE_BCE_COMBINE_BLOCK);
  BCL_clear_info_();
}

void BCL_tool_register_limit_combine(cycle_t ti, bct_id_t bct_id, step_t limit_step)
{
#if SIZE_OF_BCT_ID_T == 1
  BCL_tool_prepare_param_uint8(bct_id);
#elif SIZE_OF_BCT_ID_T == 2
  BCL_tool_prepare_param_uint16(bct_id);
#elif SIZE_OF_BCT_ID_T == 4
  BCL_tool_prepare_param_uint32(bct_id);
#else
#error Illegal value for SIZE_OF_BCT_ID_T
#endif

  BCL_tool_prepare_param_uint8((uint8_t)limit_step);

  BCL_register_cmd_(ti, Cmd_CODE_BCE_TIMELIMIT_COMBINE_BLOCK);
  BCL_clear_info_();
}

void BCL_tool_register_deploy(cycle_t ti, bct_id_t bct_id, TLCD_ID tlcd_id)
{
  BCL_tool_prepare_param_uint8((uint8_t)tlcd_id);

#if SIZE_OF_BCT_ID_T == 1
  BCL_tool_prepare_param_uint8(bct_id);
#elif SIZE_OF_BCT_ID_T == 2
  BCL_tool_prepare_param_uint16(bct_id);
#elif SIZE_OF_BCT_ID_T == 4
  BCL_tool_prepare_param_uint32(bct_id);
#else
#error Illegal value for SIZE_OF_BCT_ID_T
#endif

  BCL_register_cmd_(ti, Cmd_CODE_TLCD_DEPLOY_BLOCK);
  BCL_clear_info_();
}

void BCL_tool_register_app(cycle_t ti, AR_APP_ID app_id)
{
  BCL_register_app_(ti, app_id);
  BCL_clear_info_();
}

// TODO: prepare_param系の関数にidx超過のassertionを入れる
void BCL_tool_prepare_param_uint8(uint8_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_int8(int8_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_uint16(uint16_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_int16(int16_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_uint32(uint32_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_int32(int32_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_float(float val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_uint64(uint64_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_int64(int64_t val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}

void BCL_tool_prepare_param_double(double val)
{
  endian_memcpy(&block_command_loader_.params[block_command_loader_.param_idx], &val, sizeof(val));
  block_command_loader_.param_idx += sizeof(val);
}


void BCL_register_cmd_(cycle_t ti, CMD_CODE cmd_id)
{
  CCP_form_tlc(&block_command_loader_.packet,
               ti,
               cmd_id,
               &block_command_loader_.params[0],
               (uint16_t)block_command_loader_.param_idx);
  BCT_register_cmd(&block_command_loader_.packet);
}

void BCL_register_cmd_to_other_obc_(cycle_t ti, APID apid, CMD_CODE cmd_id)
{
  CCP_form_tlc_to_other_obc(&block_command_loader_.packet,
                            ti,
                            apid,
                            cmd_id,
                            &block_command_loader_.params[0],
                            (uint16_t)block_command_loader_.param_idx);
  BCT_register_cmd(&block_command_loader_.packet);
}

void BCL_register_app_(cycle_t ti, AR_APP_ID app_id)
{
  CCP_form_app_cmd(&block_command_loader_.packet, ti, app_id);
  BCT_register_cmd(&block_command_loader_.packet);
}

static void BCL_clear_info_(void)
{
  memset(&block_command_loader_, 0, sizeof(block_command_loader_));
}

#pragma section
