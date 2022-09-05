/**
 * @file
 * @brief モード遷移の制御の定義
 */
#ifndef MODE_MANAGER_H_
#define MODE_MANAGER_H_

#include "../TimeManager/obc_time.h"
#include <src_user/Settings/Modes/mode_definitions.h>
#include "../../TlmCmd/common_cmd_packet.h"
#include "../../TlmCmd/block_command_table.h"

#define MM_NOT_DEFINED (BCT_MAX_BLOCKS)

#include <src_user/Settings/Modes/mode_manager_params.h>

/**
 * @enum  MM_ACK
 * @brief ModeManager 関連関数の返り値
 * @note  uint8_t を想定
 */
typedef enum
{
  MM_SUCCESS,         //!< 成功
  MM_BAD_ID,          //!< モード番号異常
  MM_BAD_BC_INDEX,    //!< モード用の BC の ID 異常
  MM_INACTIVE_BLOCK,  //!< モード用 BC が 無効
  MM_OVERWRITE,       //!< 別のモード遷移実行中
  MM_ILLEGAL_MOVE,    //!< モード遷移用の BC が存在しない
  MM_NOT_IN_PROGRESS, //!< モード遷移中ではない
  MM_TL_LOAD_FAILED   //!< Task List へのモード BC 登録失敗
} MM_ACK;

/**
 * @enum MM_STATUS
 * @brief モード遷移状態
 * @note  uint8_t を想定
 */
typedef enum
{
  MM_STATUS_FINISHED,   //!< モード遷移終了
  MM_STATUS_IN_PROGRESS //!< モード遷移実行中
} MM_STATUS;

/**
 * @struct MM_TransitionEdge
 * @brief MM TLM 用の構造体
 * @note TransitionTable 全体を載せると Tlm の 1ページに収まらないため
 */
typedef struct
{
  uint8_t  from;     //!< モード遷移元
  uint8_t  to;       //!< モード遷移先
  bct_id_t bc_index; //!< モード遷移に該当する bc id
} MM_TransitionEdge;

/**
 * @struct ModeManager
 * @brief ModeManager 本体
 */
typedef struct
{
  bct_id_t mode_list[MD_MODEID_MODE_MAX];                            //!< 各モードIDに対応するブロックコマンドIDを保存する
  bct_id_t transition_table[MD_MODEID_MODE_MAX][MD_MODEID_MODE_MAX]; //!< 各モード遷移(あるモードIDからあるモードIDへの遷移)に対応するブロックコマンドを保存する
  MM_STATUS stat;                                                    //!< モード遷移状態
  MD_MODEID previous_id;                                             //!< ひとつ前のモードID
  MD_MODEID current_id;                                              //!< 現在のモードID
  MM_ACK mm_ack;                                                     //!< エラー情報保存用

  MM_TransitionEdge transition_table_for_tlm[MD_MODEID_MODE_MAX * MD_MODEID_MODE_MAX]; //!< テレメ用のテーブル
} ModeManager;

extern const ModeManager* const mode_manager;

/**
 * @brief モード遷移を管理する ModeManager 構造体 (mode_manager_) の初期化
 * @param void
 * @return void
 */
void MM_initialize(void);

/**
 * @brief TLM 用情報の生成
 * @param void
 * @return uint8_t: 遷移 Edge 本数
 * @note テレメの上部で呼び出されるようにするとテレメ生成時に勝手に情報が更新されて便利 (tlm update cmd を打つ必要が無い)
 */
uint16_t MM_update_transition_table_for_tlm(void);

/**
 * @brief Cmd_MM_SET_MODE_LISTの実体
 * @param[in] mode: 対応 Mode ID
 * @param[in] bc_index: Mode に対応する BC ID
 * @return MM_ACK
 */
MM_ACK MM_set_mode_list(MD_MODEID mode, bct_id_t bc_index);

/**
 * @brief Cmd_MM_SET_TRANSITION_TABLEの実体
 * @param[in] from: 遷移元
 * @param[in] to: 遷移先
 * @param[in] bc_index: 遷移に対応する BC ID
 * @return MM_ACK
 */
MM_ACK MM_set_transition_table(MD_MODEID from, MD_MODEID to, bct_id_t bc_index);

/**
 * @brief mode に対応した TL の BCT ID を取得する
 * @note 引数アサーションは行わない．不正な mode の場合は 0 を返す．
 * @param[in] mode: 対応するモード
 * @return モード対応する BC ID
 */
bct_id_t MM_get_tasklist_id_of_mode(MD_MODEID mode);

CCP_CmdRet Cmd_MM_SET_MODE_LIST(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MM_SET_TRANSITION_TABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MM_START_TRANSITION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MM_FINISH_TRANSITION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_MM_UPDATE_TRANSITION_TABLE_FOR_TLM(const CommonCmdPacket* packet);

#endif
