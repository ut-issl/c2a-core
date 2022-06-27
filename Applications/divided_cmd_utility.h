/**
 * @file
 * @brief コマンド分割をサポートするUtil
 * @note  HOW TO USE
 *        1. 各Cmdにて初めに DCU_check_in を実行し，現在の実行状況を取得する
 *        2. 実行が終了し 再度再帰させる必要がある場合は DCU_register_next で登録する
 *        3. すべての処理が終了した場合は DCU_report_finish を実行する
 *        4. 実行中のエラーは DCU_report_err で報告する．すると中断される
 *        5. 中断したいときは DCU_abort_cmd を実行する
 *        6. 中断された状態をリセットする場合は DCU_donw_abort_flag を実行する
 * @note  telemetry_manager などで使っているので，それを参照のこと
 */
#ifndef DIVIDED_CMD_UTILITY_H_
#define DIVIDED_CMD_UTILITY_H_

#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"
#include "../System/TimeManager/obc_time.h"
#include <src_user/TlmCmd/command_definitions.h>

#define DCU_LOG_MAX    (16)    //!< 保存するログの最大数

#include <src_user/Settings/Applications/divided_cmd_utility_params.h>

// key が uint8_t なので，それ以下を要請する
#if DCU_LOG_MAX > 255
#error "err at divided_cmd_utility"
#endif

/**
 * @enum   DCU_ACK
 * @note   uint8_t を想定
 * @brief  汎用返り値
 */
typedef enum
{
  DCU_ACK_OK = 0,       //!< 正常終了
  DCU_ACK_ERR           //!< エラー
} DCU_ACK;


/**
 * @enum   DCU_STATUS
 * @note   uint8_t を想定
 * @brief  実行状況
 */
typedef enum
{
  DCU_STATUS_FINISHED = 0,        //!< 実行終了 or 未実行
  DCU_STATUS_PROGRESS,            //!< 実行中
  DCU_STATUS_ABORTED_BY_ERR,      //!< エラーにより中断
  DCU_STATUS_ABORTED_BY_CMD       //!< コマンドにより中断
} DCU_STATUS;


/**
 * @enum   DCU_LOG_ACK
 * @note   uint8_t を想定
 * @brief  ログ操作の返り値
 */
typedef enum
{
  DCU_LOG_ACK_OK = 0,           //!< 正常終了
  DCU_LOG_ACK_NOT_FOUND         //!< 指定ログが見つからず
} DCU_LOG_ACK;


/**
 * @struct DCU_ExecStatus
 * @brief  実行状況
 */
typedef struct
{
  CMD_CODE     cmd_code;          //!< 実行コマンド
  DCU_STATUS   status;            //!< 実行状況
  uint16_t     exec_counter;      //!< 実行カウンタ．何度目の実行か？
  CCP_EXEC_STS last_exec_sts;     //!< 最終実行結果
  ObcTime      last_exec_time;    //!< 最終実行時刻
} DCU_ExecStatus;


/**
 * @struct DividedCmdUtility
 * @brief  DividedCmdUtility の AppInfo 構造体
 */
typedef struct
{
  DCU_ExecStatus exec_logs[DCU_LOG_MAX];        //!< 実行ログ．[exec_log_order[0]] が最も新しい．古いものは捨てられていく．
  uint8_t        exec_log_order[DCU_LOG_MAX];   //!< 実行ログのソートキー． exec_logs を最新順に並べ替えるために使う．
} DividedCmdUtility;

extern const DividedCmdUtility* const divided_cmd_utility;

AppInfo DCU_create_app(void);


/**
 * @brief  DCUにチェックインする．分割コマンド実行時，最初に呼び出す関数．
 * @param[in]  cmd_code:     自身の CMD_CODE
 * @param[out] exec_counter: 次の実行回数が何回目か？
 * @retval DCU_STATUS_FINISHED:       初回実行である． exec_counter は 0 を返す．
 * @retval DCU_STATUS_PROGRESS:       分割2回目以降の実行である
 * @retval DCU_STATUS_ABORTED_BY_ERR: エラーが発生しているので，実行を中止すべき（ CCP_EXEC_ILLEGAL_CONTEXT を返すべき ）
 * @retval DCU_STATUS_ABORTED_BY_CMD: 実行中断コマンドを受信しているので，実行を中止すべき（ CCP_EXEC_ILLEGAL_CONTEXT を返すべき ）
 */
DCU_STATUS DCU_check_in(CMD_CODE cmd_code, uint16_t* exec_counter);

/**
 * @brief  次の分割の実行を登録
 * @param[in] cmd_code: 自身の CMD_CODE
 * @param[in] param:    コマンドパラメタ
 * @param[in] len:      パラメタ長
 * @return DCU_ACK
 */
DCU_ACK DCU_register_next(CMD_CODE cmd_code, const uint8_t* param, uint16_t len);

/**
 * @brief  DCUに，分割コマンドが実行終了したことを伝える
 * @param[in] cmd_code: 自身の CMD_CODE
 * @param[in] last_exec_sts: コマンド実行結果
 * @return void
 */
void DCU_report_finish(CMD_CODE cmd_code, CCP_EXEC_STS last_exec_sts);

/**
 * @brief  DCUに，途中でエラーが発生したことを伝える
 * @note   以後の実行は中断される
 * @param[in] cmd_code: 自身の CMD_CODE
 * @param[in] last_exec_sts: コマンド実行結果
 * @return void
 */
void DCU_report_err(CMD_CODE cmd_code, CCP_EXEC_STS last_exec_sts);

/**
 * @brief  エラー，またはコマンドによって中断ステータスとなっているコマンドを，実行可能状態に戻す
 * @param[in] cmd_code: 実行可能状態に戻すコマンドの CMD_CODE
 * @return void
 */
void DCU_donw_abort_flag(CMD_CODE cmd_code);

/**
 * @brief  実行中の分割コマンドを外部から強制的に中断させる
 * @note   内部エラーは DCU_report_err を使うべきで，基本的には使わないはず
 * @note   abort からの復帰は，初期化される（途中からの再開は不可）
 * @param[in] cmd_code: 停止させるコマンドの CMD_CODE
 * @return void
 */
void DCU_abort_cmd(CMD_CODE cmd_code);

/**
 * @brief  ログを探して，指定したコマンドログを取得する
 * @note   exec_log は divided_cmd_utility の該当のログへのポインタに差し替える．
 * @param[in] cmd_code: 探すコマンド
 * @param[out] exec_log: 取得したログ．constポインタで，参照先を書き換えるため，NULLポインタを渡すのもOK    // FIXME: これバグってない？（ダブルポインタにすべき？）
 * @retval DCU_LOG_ACK_OK:        見つかった
 * @retval DCU_LOG_ACK_NOT_FOUND: 見つからず
 */
DCU_LOG_ACK DCU_search_and_get_log(CMD_CODE cmd_code, const DCU_ExecStatus* exec_log);

/**
 * @brief 実行中の分割コマンドを停止する
 * @note  もし，指定した Cmd が実行中ではなくても CCP_EXEC_SUCCESS を返す
 */
CCP_EXEC_STS Cmd_DCU_ABORT_CMD(const CommonCmdPacket* packet);

/**
 * @brief エラー，またはコマンドによって中断ステータスとなっているコマンドを，実行可能状態に戻す
 * @note  もし，指定した Cmd が中断ステータスではなくても CCP_EXEC_SUCCESS を返す
 */
CCP_EXEC_STS Cmd_DCU_DOWN_ABORT_FLAG(const CommonCmdPacket* packet);

/**
 * @brief ログをクリアする
 */
CCP_EXEC_STS Cmd_DCU_CLEAR_LOG(const CommonCmdPacket* packet);

#endif
