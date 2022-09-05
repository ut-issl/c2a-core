/**
 * @file
 * @brief OBC時刻のカウントアップと，各種衛星時刻関連処理
 */
#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

#include "obc_time.h"
#include "../../TlmCmd/common_cmd_packet.h"

#define TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL (1577836800.0)  /*!< 2020-01-01T00:00:00Z 時点の unixtime.
                                                                 utl_unixtime_epoch のデフォルト値 */

/**
 * @enum   TMGR_ACK
 * @brief  Time Manager の汎用返り値
 * @note   uint8_t を想定
 */
typedef enum
{
  TMGR_ACK_OK = 0,       //!< 正常終了
  TMGR_ACK_PARAM_ERR     //!< パラメタエラー
} TMGR_ACK;

/**
 * @struct TimeManager
 * @brief master_clock_ で OBC 時刻情報を保持するとともに, unixtime との紐づけや初期化情報の記録を行う
 */
typedef struct
{
  ObcTime master_clock_;
  struct
  {
    double unixtime_at_ti0;    //!< master_clock が {0, 0, 0} の時の unixtime
    cycle_t ti_at_last_update; //!< "unixtime_info_ を最後に更新した (GPSなどの) 時刻情報" を観測した時点の total_cycle
    double utl_unixtime_epoch; /*!< これを時刻ゼロとして起算した cycle 刻みの時刻を utl_unixtime と定義する.
                                    cycle 未満の精度は切り捨てられるので utl_unixtime は整数値となる.
                                    UTL_cmd の実行時刻情報として用いる. */
    double cycle_correction;   /*!< CYCLES_PER_SEC の補正倍率. unixtime <> OBCTime の変換で使う
                                    CYCLES_PER_SEC * cycle_correction = OBC のクロック誤差を反映した実際の値
                                    初期値は 1.0 */
  } unixtime_info_;
  struct
  {
    ObcTime initializing_time;
    uint8_t initializing_flag;
  } init_info_;
} TimeManager;

extern const TimeManager* const time_manager;

/**
 * @brief TimeManager 構造体を初期化する
 * @param void
 * @return void
 */
void TMGR_init(void);

/**
 * @brief master_clock_ と unixtime_info_ をクリアし全てゼロにする
 * @param void
 * @return void
 */
void TMGR_clear(void);

/**
 * @brief master_clock_ の mode_cycle をゼロにする
 * @param void
 * @return void
 */
void TMGR_clear_master_mode_cycle(void);

/**
 * @brief master_clock_ をstep刻みでインクリメントする
 * @param void
 * @return void
 */
void TMGR_count_up_master_clock(void);

/**
 * @brief C2Aの初期化終了時に呼び出し, 初期化flagを下げ, TMGRをもう一度クリアする
 * @param void
 * @return void
 */
void TMGR_down_initializing_flag(void);

/**
 * @brief 現在の master_clock_ を取得する
 * @param void
 * @retval {0, 0, 0} : C2A初期化中の場合
 * @retval master_clock_ : それ以外
 */
ObcTime TMGR_get_master_clock(void);

/**
 * @brief 初期化にかかった時間も加算した master_clock_ を返す
 * @param void
 * @return master_clock_ + initializing_time
 */
ObcTime TMGR_get_master_clock_from_boot(void);

/**
 * @brief 現在の total_cycle を返す
 * @param void
 * @return master_clock_ の total_cycle
 */
cycle_t TMGR_get_master_total_cycle(void);

/**
 * @brief 現在の mode_cycle を返す
 * @param void
 * @return master_clock_ の mode_cycle
 */
cycle_t TMGR_get_master_mode_cycle(void);

/**
 * @brief 現在の step を返す
 * @param void
 * @return master_clock_ の step
 */
step_t TMGR_get_master_step(void);

/**
 * @brief 現在の total_cycle をミリ秒単位で返す
 * @note uint32_t がオーバーフローする可能性があるので注意
 * @note 計算上はstepも考慮
 * @param void
 * @return ミリ秒単位の total_cycle
 */
uint32_t TMGR_get_master_total_cycle_in_msec(void);

/**
 * @brief 現在の mode_cycle をミリ秒単位で返す
 * @note uint32_t がオーバーフローする可能性があるので注意
 * @note 計算上はstepも考慮
 * @param void
 * @return ミリ秒単位の mode_cycle
 */
uint32_t TMGR_get_master_mode_cycle_in_msec(void);

/**
 * @brief unixtime_info_ を初期化する
 * @param void
 * @return void
 */
void TMGR_clear_unixtime_info(void);

/**
 * @brief unixtime と TI の紐づき情報を更新する
 * @param[in] unixtime (GPS 等から観測した) unixtime
 * @param[in] time (GPS 等から) unixtime を観測した時の ObcTime
 * @return TMGR_ACK
 */
TMGR_ACK TMGR_update_unixtime(const double unixtime, const ObcTime* time);

/**
 * @brief unixtime_at_ti0 を取得する
 * @param void
 * @return unixtime_at_ti0
 */
double TMGR_get_unixtime_at_ti0(void);

/**
 * @brief utl_unixtime_epoch を取得する
 * @param void
 * @return utl_unixtime_epoch
 */
double TMGR_get_utl_unixtime_epoch(void);

/**
 * @brief OBC のクロック誤差を反映した cycles_per_sec を返す
 * @param void
 * @return cycles_per_sec
 */
double TMGR_get_precise_cycles_per_sec(void);

/**
 * @brief OBC のクロック誤差を反映した正確な ti を秒単位で返す
 * @param[in] time ti を保持している OBCTime
 * @return ti（秒単位, 小数点以下も保持）
 */
double TMGR_get_precise_ti_in_sec(const ObcTime* time);

/**
 * @brief 現在の unixtime を OBC の ti をもとに計算して返す
 * @param void
 * @return unixtime
 */
double TMGR_get_current_unixtime(void);

/**
 * @brief ObcTime を unixtime に変換する
 * @param[in] ObcTime
 * @return unixtime
 */
double TMGR_get_unixtime_from_obc_time(const ObcTime* time);

/**
 * @brief UTL_cmdで用いる utl_unixtime を 一般的なunixtimeに変換する
 * @note utl_unixtime の単位としての cycle は, OBC のクロック誤差を含まない定義通りの値であることに注意
 * @param[in] utl_unixtime
 * @return unixtime
 */
double TMGR_get_unixtime_from_utl_unixtime(const cycle_t utl_unixtime);

/**
 * @brief unixtime を TI (cycle単位) に変換する
 * @param[in] unixtime
 * @retval 0  : 引数の unixtime が unixtime_at_ti0 より小さいとき
 * @retval TI : それ以外. 整数に丸めず double のまま返す
 */
double TMGR_get_precise_ti_from_unixtime(const double unixtime);

/**
 * @brief unixtime を ObcTime に変換する
 * @param[in] unixtime
 * @retval {0, 0, 0} : 引数の unixtime が unixtime_at_ti0 より小さいとき
 * @retval ObcTime   : それ以外
 */
ObcTime TMGR_get_obc_time_from_unixtime(const double unixtime);

/**
 * @brief 引数で指定された utl_unixtime に対応する TI を返す
 * @note UTL_cmd で実行時刻情報を TI に変換する際に用いる
 * @param[in] utl_unixtime
 * @retval 0 : 引数の utl_unixtime が unixtime_at_ti0 より小さい場合
 * @retval TI (total_cycleのこと) : それ以外の場合
 */
cycle_t TMGR_get_ti_from_utl_unixtime(const cycle_t utl_unixtime);

CCP_CmdRet Cmd_TMGR_SET_TIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_UPDATE_UNIXTIME(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_SET_CYCLE_CORRECTION(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_RESET_CYCLE_CORRECTION(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_TMGR_CLEAR_UNIXTIME_INFO(const CommonCmdPacket* packet);

#endif
