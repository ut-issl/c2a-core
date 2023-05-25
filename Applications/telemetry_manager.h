/**
 * @file
 * @brief userがテレメトリ詰まりをやTL溢れを防ぎつつ，またCDHなどがテレメトリを管理しやすくするためのApp
 * @note  利用方法は telemetry_manager.h の最下部を参照（FIXME: あとでdocumentに移す）
 */
#ifndef TELEMETRY_MANAGER_H_
#define TELEMETRY_MANAGER_H_

#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"
#include "../TlmCmd/common_cmd_packet_util.h"
#include "../TlmCmd/block_command_table.h"
#include <src_user/TlmCmd/telemetry_definitions.h>
#include <src_user/Settings/TlmCmd/Ccsds/apid_define.h>


// 以下がともに 10 であることで， (10 - 1 (TLM_MGR_BC_TYPE_MASTER)) x 10 x cycle で 10 秒周期に 100 個の tlm を登録できる．
#define TLM_MGR_USE_BC_NUM            (10)    //!< 用いる BC の数．基本的に 10 固定を想定
#define TLM_MGR_MAX_TLM_NUM_PER_BC    (10)    //!< 1 BC に何個のテレメ生成コマンドを登録できるか．基本的に 10 固定を想定

#if BCT_MAX_CMD_NUM < TLM_MGR_MAX_TLM_NUM_PER_BC       // FIXME: BCT 側が直ったらなおす
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
 * @brief  BC のタイプ．これにて実行種別が変わる
 */
typedef enum
{
  TLM_MGR_BC_TYPE_MASTER,             //!< 全体の BC を deploy していく BC
  TLM_MGR_BC_TYPE_HK_TLM,             //!< HK テレメ (or 全系や system で入れておきたい tlm (1 Hz))
  TLM_MGR_BC_TYPE_HIGH_FREQ_TLM,      //!< User (HIGH_FREQ) テレメ (1 Hz)
  TLM_MGR_BC_TYPE_LOW_FREQ_TLM        //!< User (LOW_FREQ) テレメ (1/10 Hz)
} TLM_MGR_BC_TYPE;


/**
 * @enum   TLM_MGR_REGISTERED_CMD_TYPE
 * @note   uint8_t を想定
 * @brief  BC に登録された（テレメ生成などの）コマンドのタイプ
 */
typedef enum
{
  TLM_MGR_REGISTERED_CMD_TYPE_UNREGISTERED = 0,   //!< まだ使われていない (これは 0 であることが必要)
  TLM_MGR_REGISTERED_CMD_TYPE_TG_GENERATE_MS_TLM,
  TLM_MGR_REGISTERED_CMD_TYPE_TG_GENERATE_ST_TLM,
  TLM_MGR_REGISTERED_CMD_TYPE_TG_GENERATE_MS_TLM,
  TLM_MGR_REGISTERED_CMD_TYPE_TG_GENERATE_ST_TLM,
  TLM_MGR_REGISTERED_CMD_TYPE_DR_REPLAY_TLM
} TLM_MGR_REGISTERED_CMD_TYPE;


/**
 * @struct TLM_MGR_BcInfo
 * @brief  BC の情報
 */
typedef struct
{
  bct_id_t        bc_id;
  TLM_MGR_BC_TYPE bc_type;
} TLM_MGR_BcInfo;


/**
 * @struct TLM_MGR_RegisterInfo
 * @brief  どう BC を管理し，テレメ生成コマンドを登録し，動かしていくかの情報
 * @note   TLM_MGR_BcInfo の情報から生成される
 */
typedef struct
{
  uint8_t bc_info_idxes[TLM_MGR_USE_BC_NUM];    //!< bc_infos のどの idx の BC を使うか．static 確保のため，最大数 TLM_MGR_USE_BC_NUM の配列を確保
  uint8_t bc_info_idxes_size;                   //!< bc_info_idxes の配列数
  uint8_t registered_tlm_num;                   /*!< すでに登録されているテレメ数．この値から一意に次にコマンドを生成する BCT_Pos.cmd が決まる
                                                     コマンドは隙間なく前から詰め込まれている，という想定 */
} TLM_MGR_RegisterInfo;


/**
 * @struct TLM_MGR_RegisteredCmdTableInBlock
 * @brief  BC に登録された（テレメ生成などの）コマンドの情報（BC の block ごと）
 * @note   コマンドは隙間なく前から詰め込まれている，という想定
 */
typedef struct
{
  struct
  {
    TLM_MGR_REGISTERED_CMD_TYPE cmd_type;   //!< BC に登録された（テレメ生成などの）コマンドのタイプ （未登録は TLM_MGR_REGISTERED_CMD_TYPE_UNREGISTERED (0)）
    APID apid;                              //!< 登録された tlm 生成コマンドの APID （未登録は 0）
    TLM_CODE tlm_id;                        /*!< 登録された tlm 生成コマンドの生成 tlm id （未登録は 0）
                                                 なお， DR 再生 Cmd_DR_REPLAY_TLM のときは， dr_partition がはいる */
  } register_infos[TLM_MGR_MAX_TLM_NUM_PER_BC];
} TLM_MGR_RegisteredCmdTableInBlock;


/**
 * @struct TLM_MGR_RegisteredCmdTable
 * @brief  BC に登録された（テレメ生成などの）コマンドの情報
 */
typedef struct
{
  TLM_MGR_RegisteredCmdTableInBlock cmd_infos[TLM_MGR_USE_BC_NUM];      //!< BC の各 Block ごとの情報
} TLM_MGR_RegisteredCmdTable;


/**
 * @struct TelemetryManager
 * @brief  TelemetryManager の AppInfo構造体
 */
typedef struct
{
  TLM_MGR_BcInfo bc_infos[TLM_MGR_USE_BC_NUM];  //!< TLM_MGR_BcInfo 配列．利用する BC の情報．
  struct
  {
    // FIXME: master 入らないのでは？ 追加登録されるわけでもないし ということで．消す．
    TLM_MGR_RegisterInfo master;      //!< TLM_MGR_BC_TYPE_MASTER; 全体の BC を deploy していく BC
    TLM_MGR_RegisterInfo hk;          /*!< TLM_MGR_BC_TYPE_HK_TLM; HK テレメ (or 全系や system で入れておきたい tlm (1 Hz))
                                           userのtlm消去．追加の影響を受けない
                                           これによって，tlmの全体管理が容易になる  */
    TLM_MGR_RegisterInfo high_freq;   //!< TLM_MGR_BC_TYPE_HIGH_FREQ_TLM; User テレメ (1 Hz)
    TLM_MGR_RegisterInfo low_freq;    //!< TLM_MGR_BC_TYPE_LOW_FREQ_TLM; User テレメ (1/10 Hz)
  } register_info;
  bct_id_t master_bc_id;              //!< TLM_MGR_BC_TYPE_MASTER に登録されている BC ID
  TLM_MGR_RegisteredCmdTable registered_cmd_table;  //!< 現在 BC に登録された（テレメ生成などの）コマンド
  uint8_t is_inited;                  //!< 初期化されているか？
} TelemetryManager;


extern const TelemetryManager* const telemetry_manager;

AppInfo TLM_MGR_create_app(void);


/**
 * @brief 初期化
 * @note  DCU を使っているので，完了に 1 秒ほどかかる
 */
CCP_CmdRet Cmd_TLM_MGR_INIT(const CommonCmdPacket* packet);

/**
 * @brief master bc の初期化
 */
CCP_CmdRet Cmd_TLM_MGR_INIT_MASTER_BC(const CommonCmdPacket* packet);

/**
 * @brief HK テレメを初期化
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_HK_TLM(const CommonCmdPacket* packet);

/**
 * @brief HIGH_FREQ テレメを初期化
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_HIGH_FREQ_TLM(const CommonCmdPacket* packet);

/**
 * @brief LOW_FREQ テレメを初期化
 */
CCP_CmdRet Cmd_TLM_MGR_CLEAR_LOW_FREQ_TLM(const CommonCmdPacket* packet);

/**
 * @brief User テレメ (HIGH_FREQ, LOW_FREQ) を初期化
 * @note  DCU を使っているので，完了に 0.2 秒ほどかかる
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

// 以下，コマンド登録コマンド
CCP_CmdRet Cmd_TLM_MGR_REGISTER_GENERATE_MS_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_REGISTER_GENERATE_ST_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_REGISTER_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_REGISTER_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_REGISTER_REPLAY_TLM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TLM_MGR_DELETE_GENERATE_MS_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_DELETE_GENERATE_ST_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_DELETE_FORWARD_AS_MS_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_DELETE_FORWARD_AS_ST_TLM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_TLM_MGR_DELETE_REPLAY_TLM(const CommonCmdPacket* packet);

// TODO: いきなり設定が変わるのではなく，設定変更 → 反映，にしたい．
// CCP_CmdRet Cmd_TLM_MGR_APPLY(const CommonCmdPacket* packet);


// *** HOW TO USE ***
// FIXME
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
