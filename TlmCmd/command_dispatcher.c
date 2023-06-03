#pragma section REPRO
/**
 * @file
 * @brief 各種コマンドの実行管理
 */
#include "command_dispatcher.h"
#include <src_user/TlmCmd/command_definitions.h>
#include "../System/TimeManager/time_manager.h"
#include "../System/EventManager/event_logger.h"
#include "packet_handler.h"

// TODO: 本当は，不正な CDISは pl == NULL などにしておくのが良さそうだが，
//       現状 PL が NULL チェックをしてないので，できない

/**
 * @enum  CDIS_EL_LOCAL_ID
 * @brief CDIS 内部の event の local ID
 * @note  uint8_t
 */
typedef enum
{
  CDIS_EL_LOCAL_ID_NULL_PARAM,    //!< NULL 引数
  CDIS_EL_LOCAL_ID_INVALID_PL,    //!< 不正な PL
  CDIS_EL_LOCAL_ID_UNKNOWN
} CDIS_EL_LOCAL_ID;

/**
 * @brief  CDIS_ExecInfo の初期化
 * @param[in,out] exec_info: 初期化する exec_info
 * @return void
 */
static void CDIS_clear_exec_info_(CDIS_ExecInfo* exec_info);


CommandDispatcher CDIS_init(PacketList* pl)
{
  CommandDispatcher cdis;
  static uint8_t init_counter = 0;

  cdis.idx = init_counter;
  init_counter++;

  // コマンド実行情報を初期化
  CDIS_clear_exec_info_(&cdis.prev);
  CDIS_clear_exec_info_(&cdis.prev_err);

  // 実行エラーカウンタを0に初期化
  cdis.error_counter = 0;

  // 実行中断フラグを無効に設定
  cdis.lockout = 0;

  // 異常時実行中断フラグを無効に設定
  cdis.stop_on_error = 0;

  // 処理対象とするPacketListをクリアして登録
  if (pl == NULL)
  {
    // 初期化時エラーは試験時に確認され，打ち上げ後はありえないので，イベント発行のみしかしない
    EL_record_event((EL_GROUP)EL_CORE_GROUP_CDIS_INTERNAL_ERR,
                    CDIS_EL_LOCAL_ID_NULL_PARAM,
                    EL_ERROR_LEVEL_HIGH,
                    0);
    return cdis;
  }
  if (PL_get_packet_type(pl) != PL_PACKET_TYPE_CCP)
  {
    // 初期化時エラーは試験時に確認され，打ち上げ後はありえないので，イベント発行のみしかしない
    EL_record_event((EL_GROUP)EL_CORE_GROUP_CDIS_INTERNAL_ERR,
                    CDIS_EL_LOCAL_ID_INVALID_PL,
                    EL_ERROR_LEVEL_HIGH,
                    (uint32_t)pl);
    return cdis;
  }
  PL_clear_list(pl);
  cdis.pl = pl;

  return cdis;
}


static void CDIS_clear_exec_info_(CDIS_ExecInfo* exec_info)
{
  OBCT_clear(&(exec_info->time));
  exec_info->code = (CMD_CODE)0;
  exec_info->cmd_ret = CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


void CDIS_dispatch_command(CommandDispatcher* cdis)
{
  static CommonCmdPacket packet_; // パケットコピー用．サイズが大きいため静的変数として宣言

  // 実行有効フラグが無効化されている場合は処理打ち切り
  if (cdis->lockout) return;

  // 実行すべきコマンドが無い場合は処理終了
  if (cdis->pl == NULL) return;       // TODO: PL 側で NULL チェックに対応したら消す
  if (PL_is_empty(cdis->pl)) return;

  if (cdis->prev.cmd_ret.exec_sts != CCP_EXEC_SUCCESS)
  {
    // 直前コマンドが異常終了した場合は実行前に情報を保存
    // 実行前にコピーすることで次コマンドが異常終了した場合は
    // prev と prev_err で2コマンド分の異常情報を保持できる
    cdis->prev_err = cdis->prev;
  }

  // 実行すべきコマンドパケットを取得
  packet_ = *(const CommonCmdPacket*)(PL_get_head(cdis->pl)->packet);

  // ここで実行種別を変更するのをやめた．
  // - MOBCから配送される第二OBCにも，GS cmdやTL cmdを送信したいため
  // - user_packet_handler での PH_user_cmd_router の dispatcher (AOBC_dispatch_command など) にて
  //   第二OBCが受けたいコマンド種別へと変換させる．
  /*
  // 実行時は全てのコマンドの実行種別をRealTimeに設定する。
  // タイムラインはここでリアルタイムに変換される。
  // この処理は特に複数機器でパケットルーティングを行う場合重要。
  // 普通はルーティング先はルーティング元のタイムラインを受け付けないはず。
  CCP_set_exec_type(&packet_, CCP_EXEC_TYPE_RT);
  */

  // 実行時情報を記録しつつコマンドを実行
  cdis->prev.time    = TMGR_get_master_clock();
  cdis->prev.code    = CCP_get_id(&packet_);
  cdis->prev.cmd_ret = PH_dispatch_command(&packet_);

  // 実行したコマンドをリストから破棄
  PL_drop_executed(cdis->pl);

  if (cdis->prev.cmd_ret.exec_sts != CCP_EXEC_SUCCESS)
  {
    uint32_t note;
    // 実行時エラー情報をELにも記録. エラー発生場所(GSCD,TLCDなど)は cdis の idx で区別
    // より重要な EL_CORE_GROUP_CDIS_EXEC_ERR_STS があとに来るように EL 発行
    EL_record_event((EL_GROUP)EL_CORE_GROUP_CDIS_EXEC_ERR_CODE,
                    (uint32_t)cdis->prev.code,
                    EL_ERROR_LEVEL_LOW,
                    cdis->prev.cmd_ret.err_code);
    note = ((0X000000ff & cdis->idx) << 24)
         | ((0x000000ff & cdis->prev.cmd_ret.exec_sts) << 16)
         | (0x0000ffff & cdis->prev.cmd_ret.err_code);
    EL_record_event((EL_GROUP)EL_CORE_GROUP_CDIS_EXEC_ERR_STS,
                    (uint32_t)cdis->prev.code,
                    EL_ERROR_LEVEL_LOW,
                    note);

    // 実行したコマンドが実行異常ステータスを返した場合
    // エラー発生カウンタをカウントアップ
    ++(cdis->error_counter);

    if (cdis->stop_on_error == 1)
    {
      // 異常時実行中断フラグが有効な場合
      // 実行許可フラグを無効化し以降の実行を中断
      cdis->lockout = 1;
    }
  }
}


void CDIS_clear_command_list(CommandDispatcher* cdis)
{
  // 保持しているリストの内容をクリア
  if (cdis->pl == NULL) return;       // TODO: PL 側で NULL チェックに対応したら消す
  PL_clear_list(cdis->pl);
}


void CDIS_clear_error_status(CommandDispatcher* cdis)
{
  // 実行エラー状態を初期状態に復元
  CDIS_clear_exec_info_(&cdis->prev_err);

  // 積算エラー回数を0クリア
  cdis->error_counter = 0;
}

#pragma section
