/** 
  * @file 
  * @brief OBCの時刻情報を TimeManager 構造体に保持しカウントアップする. その他, 時刻演算に必要な関数も実装する
  */

#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

#include "obc_time.h"
#include "../../CmdTlm/common_tlm_cmd_packet.h"

#define TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL (1577836800.0) /*!< 2020-01-01T00:00:00Z 時点のunixtime
                                                                これを時刻ゼロとして起算した0.1秒刻みの時刻をutl_unixtimeと定義し,
                                                                UTL_cmdの実行時刻情報として用いる */

typedef struct
{
  ObcTime master_clock_;
  OBCT_UnixtimeInfo unixtime_info_;
  double utl_unixtime_epoch_;
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
step_t  TMGR_get_master_step(void);

/**
 * @brief 現在の total_cycle をミリ秒単位で返す
 * @note uint32_t がオーバーフローする可能性があるので注意
 * @note 計算上はstepも考慮
 * @param void
 * @return ミリ秒単位の total_cycle
 */
uint32_t TMGR_get_master_total_cycle_in_msec(void);

/**
 * @brief 現在の total_cycle をミリ秒単位で返す
 * @note uint32_t がオーバーフローする可能性があるので注意
 * @note 計算上はstepも考慮
 * @return ミリ秒単位の total_cycle
 */
uint32_t TMGR_get_master_mode_cycle_in_msec(void);

/**
 * @brief unixtime_info_ を取得する
 * @param void
 * @return unixtime_info_
 */
OBCT_UnixtimeInfo TMGR_get_obct_unixtime_info(void);

/**
 * @brief ObcTime を unixtime に変換する
 * @param[in] ObcTime
 * @return unixtime (秒単位, 小数点以下も保持)
 */
double TMGR_get_unixtime_from_obc_time(const ObcTime* time);

/**
 * @brief unixtime を ObcTime に変換する
 * @note 引数の unixtime が unixtime_at_ti0 より小さい場合は {0, 0, 0} を返す
 * @param[in] unixtime
 * @return ObcTime
 */
ObcTime TMGR_get_obc_time_from_unixtime(const double unixtime);

/**
 * @brief 一般的なunixtimeを, UTL_cmdで用いる utl_unixtime に変換する
 * @note 引数の unixtime が utl_unixtime_epoch_ より小さい場合は 0 を返す
 * @param[in] unixtime 変換したい unixtime
 * @return utl_unixtime (デフォルトでは2020-01-01T00:00:00Z 起算, cycle刻み)
 */
cycle_t TMGR_get_utl_unixtime_from_unixtime(const double unixtime);

/**
 * @brief 引数で指定された utl_unixtime に対応する ti を返す
 * @note UTL_cmd で実行時刻情報を ti に変換する際に用いる
 * @param[in] utl_unixtime
 * @return ti (total_cycleのこと)
 */
cycle_t TMGR_get_ti_from_utl_unixtime(const cycle_t utl_unixtime);

/**
 * @brief unixtime_info_ を観測情報を用いて更新する
 * @param[in] unixtime (GPS 等から観測した) unixtime
 * @param[in] time (GPS 等から) unixtime を観測した時の ObcTime
 */
void TMGR_update_unixtime_info(const double unixtime, const ObcTime* time);

CCP_EXEC_STS Cmd_TMGR_SET_TIME(const CTCP* packet);
CCP_EXEC_STS Cmd_TMGR_SET_UNIXTIME(const CTCP* packet);

#endif
