#pragma section REPRO
/**
 * @file
 * @brief 各種コマンドの実行管理
 */
#include "command_dispatcher.h"

#include <src_user/TlmCmd/command_definitions.h>
#include "../System/TimeManager/time_manager.h"
#include "packet_handler.h"

static CDIS_ExecInfo CDIS_init_exec_info_(void);

CommandDispatcher CDIS_init(PacketList* pli)
{
  CommandDispatcher cdis;

  // コマンド実行情報を初期化。
  cdis.prev = CDIS_init_exec_info_();
  cdis.prev_err = CDIS_init_exec_info_();

  // 実行エラーカウンタを0に初期化。
  cdis.error_counter = 0;

  // 実行中断フラグを無効に設定
  cdis.lockout = 0;

  // 異常時実行中断フラグを無効に設定。
  cdis.stop_on_error = 0;

  // 処理対象とするPacketListをクリアして登録。
  PL_clear_list(pli);
  cdis.pli = pli;

  return cdis;
}

static CDIS_ExecInfo CDIS_init_exec_info_(void)
{
  CDIS_ExecInfo cei;

  OBCT_clear(&cei.time);
  cei.time.step = 0;            // ここで重複してClearしている理由はなんだ．．．
  cei.code = (CMD_CODE)0;
  cei.sts = CCP_EXEC_SUCCESS;

  return cei;
}

void CDIS_dispatch_command(CommandDispatcher* cdis)
{
  // パケットコピー用。サイズが大きいため静的変数として宣言。
  static CommonCmdPacket packet_;

  if (cdis->lockout)
  {
    // 実行有効フラグが無効化されている場合は処理打ち切り。
    return;
  }

  if (PL_is_empty(cdis->pli))
  {
    // 実行すべきコマンドが無い場合は処理終了。
    return;
  }

  if (cdis->prev.sts != CCP_EXEC_SUCCESS)
  {
    // 直前コマンドが異常終了した場合は実行前に情報を保存。
    // 実行前にコピーすることで次コマンドが異常終了した場合は
    // prevとprev_errで2コマンド分の異常情報を保持できる。
    cdis->prev_err = cdis->prev;
  }

  // 実行すべきコマンドパケットを取得。
  packet_ = *(const CommonCmdPacket*)(PL_get_head(cdis->pli)->packet);

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

  // 実行時情報を記録しつつコマンドを実行。
  cdis->prev.time = TMGR_get_master_clock();
  cdis->prev.code = CCP_get_id(&packet_);
  cdis->prev.sts  = PH_dispatch_command(&packet_);

  // 実行したコマンドをリストから破棄
  PL_drop_executed(cdis->pli);

  if (cdis->prev.sts != CCP_EXEC_SUCCESS)
  {
    // 実行したコマンドが実行異常ステータスを返した場合。
    // エラー発生カウンタをカウントアップ。
    ++(cdis->error_counter);

    if (cdis->stop_on_error == 1)
    {
      // 異常時実行中断フラグが有効な場合。
      // 実行許可フラグを無効化し以降の実行を中断。
      cdis->lockout = 1;
    }
  }
}

void CDIS_clear_command_list(CommandDispatcher* cdis)
{
  // 保持しているリストの内容をクリア
  PL_clear_list(cdis->pli);
}

void CDIS_clear_error_status(CommandDispatcher* cdis)
{
  // 実行エラー状態を初期状態に復元
  cdis->prev_err = CDIS_init_exec_info_();

  // 積算エラー回数を0クリア
  cdis->error_counter = 0;
}
#pragma section
