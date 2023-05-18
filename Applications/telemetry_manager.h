/**
 * @file
 * @brief userがテレメトリ詰まりをやTL溢れを防ぎつつ，またCDHなどがテレメトリを管理しやすくするためのApp
 * @note  https://gitlab.com/ut_issl/c2a/c2a_core_oss/-/issues/81 や telemetry_manager.h の最下部を参照（FIXME: あとでdocumentに移す）
 */
#ifndef TELEMETRY_MANAGER_H_
#define TELEMETRY_MANAGER_H_

#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"
#include "../TlmCmd/common_cmd_packet_util.h"
#include "../TlmCmd/block_command_table.h"


#define TLM_MGR_USE_BC_NUM            (10)    //!< 用いるBCの数．基本的に10固定を想定
#define TLM_MGR_MAX_TLM_NUM_PER_BC    (10)    //!< 1BCに何個のテレメ生成コマンドを登録できるか．基本的に10固定を想定


#if BCT_MAX_CMD_NUM < TLM_MGR_MAX_TLM_NUM_PER_BC       // FIXME: BCT側が直ったらなおす
#error BCT_MAX_CMD_NUM is not enough for TelemetryManager
#endif


/**
 * @enum   TLM_MGR_ERR_CODE
 * @note   uint8_t を想定
 * @brief  汎用エラーコード
 */
typedef enum
{
  TLM_MGR_ERR_CODE_OK,
  TLM_MGR_ERR_CODE_MASTER_DUPLICATED,      //!< BC 実行の masterとなる BC が重複している
  TLM_MGR_ERR_CODE_MASTER_IS_ABSENT,       //!< BC 実行の masterとなる BC が存在しない
  TLM_MGR_ERR_CODE_REGISTER_INFO_BC_FULL,  //!< RegisterInfo にこれ以上 BC を登録できない
  TLM_MGR_ERR_CODE_CMD_FULL,               //!< これ以上テレメ生成コマンドを登録できない
  TLM_MGR_ERR_CODE_OTHER_ERR               //!< その他のエラー
} TLM_MGR_ERR_CODE;


/**
 * @enum   TLM_MGR_BC_TYPE
 * @note   uint8_t を想定
 * @brief  BCのタイプ．これにて実行種別が変わる
 */
typedef enum
{
  TLM_MGR_BC_TYPE_MASTER,
  TLM_MGR_BC_TYPE_HK_TLM,
  TLM_MGR_BC_TYPE_SYSTEM_TLM,
  TLM_MGR_BC_TYPE_HIGH_FREQ_TLM,
  TLM_MGR_BC_TYPE_LOW_FREQ_TLM,
  TLM_MGR_BC_TYPE_RESERVE
} TLM_MGR_BC_TYPE;


/**
 * @struct TLM_MGR_BcInfo
 * @brief  BCの情報
 */
typedef struct
{
  bct_id_t        bc_id;
  TLM_MGR_BC_TYPE bc_type;
} TLM_MGR_BcInfo;


/**
 * @struct TLM_MGR_RegisterInfo
 * @brief  どうBCを管理し，テレメ生成コマンドを登録し，動かしていくかの情報
 * @note   TLM_MGR_BcInfo の情報から生成される
 */
typedef struct
{
  uint8_t bc_info_idx[TLM_MGR_USE_BC_NUM];             //!< bc_info のどの idx の BC を使うか．static確保のため，最大数 TLM_MGR_USE_BC_NUM の配列を確保
  uint8_t bc_info_idx_used_num;                        //!< 使用している BC 数
  uint8_t tlm_register_pointer_to_idx_of_bc_info_idx;  //!< 次にテレメ生成コマンドを登録した場合に使用される bc_info_idx の配列 idx
  uint8_t tlm_register_pointer_to_bct_cmd_pos;         //!< 次にテレメ生成コマンドを登録した場合に登録する BCT_Pos.cmd
} TLM_MGR_RegisterInfo;


/**
 * @struct TelemetryManager
 * @brief  TelemetryManager の AppInfo構造体
 */
typedef struct
{
  TLM_MGR_BcInfo bc_info[TLM_MGR_USE_BC_NUM];
  struct
  {
    TLM_MGR_RegisterInfo master;          //!< BC全体を展開していく master BC
    TLM_MGR_RegisterInfo hk_tlm;          //!< HK テレメ（1 Hz）
    TLM_MGR_RegisterInfo system_tlm;      /*!< 全系やsystemで入れておきたいtlm（1 Hz）
                                               userのtlm消去．追加の影響を受けない
                                               これによって，tlmの全体管理が容易になる  */
    TLM_MGR_RegisterInfo high_freq_tlm;   //!< user テレメ（1 Hz）
    TLM_MGR_RegisterInfo low_freq_tlm;    //!< user テレメ（1/10 Hz）
    TLM_MGR_RegisterInfo reserve;         //!< 現状使ってないBC．浮くのが嫌なので，メンバは作るがテレメ生成コマンドは登録されない
  } register_info;
  bct_id_t master_bc_id;
  uint8_t is_inited;                      //!< 初期化されているか？
} TelemetryManager;


extern const TelemetryManager* const telemetry_manager;

AppInfo TLM_MGR_create_app(void);


/**
 * @brief 初期化
 */
CCP_CmdRet Cmd_TLM_MGR_INIT(const CommonCmdPacket* packet);

/**
 * @brief master bc の初期化
 */
CCP_CmdRet Cmd_TLM_MGR_INIT_MASTER_BC(const CommonCmdPacket* packet);

/**
 * @brief HKテレメを初期化
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_HK_TLM(const CommonCmdPacket* packet);

/**
 * @brief systemテレメを初期化
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_SYSTEM_TLM(const CommonCmdPacket* packet);

/**
 * @brief high_freq_tlm, low_freq_tlm を初期化
 * @note  便宜上 TLM_MGR_BC_TYPE_RESERVE の BC も初期化してしまう
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_USER_TLM(const CommonCmdPacket* packet);

/**
 * @brief TLM送出開始
 * @note  master bc の末尾を Cmd_TLCD_DEPLOY_BLOCK にして deploy block しているだけ
 */
CCP_CmdRet Cmd_TLM_MGR_START_TLM(const CommonCmdPacket* packet);

/**
 * @brief TLM送出一時停止
 * @note  master bc の末尾の Cmd_TLCD_DEPLOY_BLOCK を NOP に差し替えているだけ
 * @note  Cmd_TLCD_CLEAR_ALL_TIMELINE / Cmd_TLM_MGR_CLEAR_TLM_TL だと他のものも消えてしまう
 * @note  Cmd_TLCD_CLEAR_ALL_TIMELINE / Cmd_TLM_MGR_CLEAR_TLM_TL のほうが適切な場合もあるのでよく考えること
 */
CCP_CmdRet Cmd_TLM_MGR_STOP_TLM(const CommonCmdPacket* packet);

/**
 * @brief TLM送出用TLをクリア
 * @note  Cmd_TLCD_CLEAR_ALL_TIMELINE しているだけ
 * @note  Cmd_TLCD_CLEAR_ALL_TIMELINE をGSから送ってもいいが， TL No がマジックナンバーになるので．
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_TLM_TL(const CommonCmdPacket* packet);

/**
 * @brief HKテレメを登録
 */
CCP_CmdRet Cmd_TLM_MGR_REGISTER_HK_TLM(const CommonCmdPacket* packet);

/**
 * @brief systemテレメを登録
 */
CCP_CmdRet Cmd_TLM_MGR_REGISTER_SYSTEM_TLM(const CommonCmdPacket* packet);

/**
 * @brief high_freq_tlm を登録
 */
CCP_CmdRet Cmd_TLM_MGR_REGISTER_HIGH_FREQ_TLM(const CommonCmdPacket* packet);

/**
 * @brief low_freq_tlm を登録
 */
CCP_CmdRet Cmd_TLM_MGR_REGISTER_LOW_FREQ_TLM(const CommonCmdPacket* packet);


// TODO: いきなり設定が変わるのではなく，設定変更 → 反映，にしたい．
// CCP_CmdRet Cmd_TLM_MGR_APPLY(const CommonCmdPacket* packet);


// *** HOW TO USE ***
/*
1. 初期化：Cmd_TLM_MGR_INIT
  - BCをいじるため，Appの初期化では現時点でできないので，明示的に初期化する必要あり
    - initial SLにいれてもいいかも
2. 非ユーザーテレメの登録
  - HK, SYSTEM TLMを登録しておく ( Cmd_TLM_MGR_REGISTER_HOGE で )
3. テレメ送出開始
  - 初回は念の為 Cmd_TLM_MGR_CLEAR_TLM_TL をして，既存のTL2を消す
  - Cmd_TLM_MGR_START_TLM にてテレメ送出開始
    - 止めるのは Cmd_TLM_MGR_STOP_TLM
4. Cmd_TLM_MGR_REGISTER_HIGH_FREQ_TLM / Cmd_TLM_MGR_REGISTER_LOW_FREQ_TLM にて，1 Hz, 1/10 Hz のユーザーテレメを各人が好きに登録できる
5. 試験が終わったら Cmd_TLM_MGR_CLEAR_USER_TLM をして，ユーザーテレメを消す
6. 次に使う人がいたら，4.から繰り返す

*/

#endif
