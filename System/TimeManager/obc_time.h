#ifndef OBC_TIME_H_
#define OBC_TIME_H_

#include <src_user/Library/stdint.h>

// step, cycleについてはTimeManagerを参照のこと
#define OBCT_STEP_IN_MSEC (1)                                                    //!< 1stepで何msか
#define OBCT_STEPS_PER_CYCLE (100)                                               //!< 何stepで1cycleか
#define OBCT_CYCLES_PER_SEC (1000 / OBCT_STEP_IN_MSEC / OBCT_STEPS_PER_CYCLE)    //!< 1sで何cycleか
#define OBCT_MAX_CYCLE (0xfffffff0u)                                             //!< 最大cycle数．つまりTIがいくつでオーバーフローするか
/**
 * @def
 * 2020-1-1-00:00:00UTC 時点のunixtime
 * これを時刻ゼロとして起算した0.1秒刻みのunixtimeをutl_unixtimeと定義し, UTL_cmdの実行時刻情報として用いる
 */
#define OBCT_UNIXTIME_EPOCH_FOR_UTL (1577836800.0)

#include <src_user/Settings/System/obc_time_params.h>

typedef uint32_t cycle_t;
typedef uint32_t step_t;

/**
 * @struct ObcTime
 * @brief OBCの時刻情報を保持する構造体
 */
typedef struct
{
  cycle_t total_cycle; //!< OBCが起動してから（正確には，構造体が初期化されてから）の経過時間 (cycle)
  cycle_t mode_cycle;  //!< 最後にmodeを変えてからの経過時間 (cycle)
  step_t  step;        //!< 内部step．TimeLineでの処理時間分割に使われる (step)
} ObcTime;

/**
 * @struct OBCT_UnixtimeInfo
 * @brief unixtimeとObcTimeを紐づける構造体
 */
typedef struct
{
  double unixtime_at_ti0;    //!< 観測情報から計算した, master_clock が {0, 0, 0} の時の unixtime
  cycle_t ti_at_last_update; //!< 最後に更新した時に使ったunixtimeの情報を(GPSなどから)観測した時点のtotal_cycle
} OBCT_UnixtimeInfo;

ObcTime OBCT_create(cycle_t total_cycle,
                    cycle_t mode_cycle,
                    step_t step);
void OBCT_clear(ObcTime* time);
void OBCT_count_up(ObcTime* time);
ObcTime OBCT_get_max(void);
cycle_t OBCT_get_total_cycle(const ObcTime* time);
cycle_t OBCT_get_mode_cycle(const ObcTime* time);
step_t  OBCT_get_step(const ObcTime* time);
uint32_t OBCT_get_total_cycle_in_msec(const ObcTime* time);  // 計算上はstepも考慮
uint32_t OBCT_get_mode_cycle_in_msec(const ObcTime* time);   // 計算上はstepも考慮
float    OBCT_get_total_cycle_in_sec(const ObcTime* time);   // 計算上はstepも考慮（オーバーフローに注意）
float    OBCT_get_mode_cycle_in_sec(const ObcTime* time);    // 計算上はstepも考慮（オーバーフローに注意）
cycle_t  OBCT_msec2cycle(uint32_t msec);      // 適当に丸められることに注意
uint32_t OBCT_cycle2msec(cycle_t cycle);      // 適当に丸められることに注意
cycle_t  OBCT_sec2cycle(uint32_t sec);        // 適当に丸められることに注意
uint32_t OBCT_cycle2sec(cycle_t cycle);       // 適当に丸められることに注意

/**
 * @brief ObcTime の引き算を行う
 * @param[in] before より古い時刻のObcTime
 * @param[in] after  より新しい時刻のObcTime
 * @return after から before を引いた結果の ObcTime
 */
ObcTime OBCT_diff(const ObcTime* before,
                  const ObcTime* after);
step_t OBCT_diff_in_step(const ObcTime* before,
                         const ObcTime* after);
uint32_t OBCT_diff_in_msec(const ObcTime* before,
                           const ObcTime* after);
float OBCT_diff_in_sec(const ObcTime* before,
                       const ObcTime* after);

/**
 * @brief ObcTime の足し算を行う
 * @param[in] left  一項目
 * @param[in] right 二項目
 * @return 引数を足し合わせた結果の ObcTime
 */
ObcTime OBCT_add(const ObcTime* left, const ObcTime* right);

/**
 * @brief ObcTime の比較
 * @note  mode_cycle はみない
 * @param[in] t1: 比較対象
 * @param[in] t2: 比較対象
 * @retval 1  : t1 < t2
 * @retval 0  : t1 == t2
 * @retval -1 : t2 > t1
 */
int OBCT_compare(const ObcTime* t1, const ObcTime* t2);

void OBCT_print(const ObcTime* time);

/**
 * @brief 観測した UnixTime とそれを観測した時の ObcTime を受け取り OBCT_UnixtimeInfo を作成する
 * @param[in] time (GPS 等から) unixtime を観測した時の ObcTime
 * @param[in] unixtime (GPS 等から観測した) unixtime
 * @return OBCT_UnixtimeInfo 引数から計算した OBCT_UnixtimeInfo
 */
OBCT_UnixtimeInfo OBCT_create_unixtime_info(const double unixtime, const ObcTime* time);

/**
 * @brief OBCT_UnixtimeInfo クリア関数, 全て0にする
 * @param[in] uti クリアする OBCT_UnixtimeInfo
 */
void OBCT_clear_unixtime_info(OBCT_UnixtimeInfo* uti);

/**
 * @brief 一般的なunixtimeを, UTL_cmdで用いる utl_unixtime に変換する
 * @param[in] unixtime 変換したい unixtime
 * @return utl_unixtime (2020-1-1-00:00:00UTC 起算, 0.1秒刻みなので単位はcycle_t)
 */
cycle_t OBCT_get_utl_unixtime_from_unixtime(const double unixtime);

/**
 * @brief OBCT_UnixtimeInfo を更新する
 * @param[in] uti 更新する OBCT_UnixtimeInfo
 * @param[in] unixtime (GPS 等から観測した) unixtime
 * @param[in] time (GPS 等から) unixtime を観測した時の ObcTime
 */
void OBCT_update_unixtime_info(OBCT_UnixtimeInfo* uti, const double unixtime, const ObcTime time);

#endif
