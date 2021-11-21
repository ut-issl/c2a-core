#ifndef OBC_TIME_H_
#define OBC_TIME_H_

#include <src_user/Library/stdint.h>

// step, cycleについてはTimeManagerを参照のこと
#define OBCT_STEP_IN_MSEC (1)                                                    //!< 1stepで何msか
#define OBCT_STEPS_PER_CYCLE (100)                                               //!< 何stepで1cycleか
#define OBCT_CYCLES_PER_SEC (1000 / OBCT_STEP_IN_MSEC / OBCT_STEPS_PER_CYCLE)    //!< 1sで何cycleか
#define OBCT_MAX_CYCLE (0xfffffff0u)                                             //!< 最大cycle数．つまりTIがいくつでオーバーフローするか

#include <src_user/Settings/System/obc_time_params.h>

typedef uint32_t cycle_t;
typedef uint32_t step_t;

typedef struct
{
  cycle_t total_cycle;   // OBCが起動してから（正確には，構造体が初期化されてから）の経過時間 (cycle)
  cycle_t mode_cycle;    // 最後にmodeを変えてからの経過時間 (cycle)
  step_t  step;          // 内部step．TimeLineでの処理時間分割に使われる (step)
} ObcTime;

/**
 * @brief unix_timeとObcTimeを紐づける構造体
 */
typedef struct
{
  double unix_time_on_ti0; //!< 観測情報から計算したmaster_clockが(0, 0)の時のunix_time
  cycle_t last_update_ti;  //!< 最後に更新した時に使ったunix_timeの情報を(GPSなどから)観測した時のObcTime
} OBCT_UnixTimeInfo;

ObcTime OBCT_create(cycle_t total_cycle,
                    cycle_t mode_cycle,
                    step_t step);
ObcTime OBCT_get_max(void);
void OBCT_clear(ObcTime* time);
void OBCT_count_up(ObcTime* time);
cycle_t OBCT_get_total_cycle(const ObcTime* time);
cycle_t OBCT_get_mode_cycle(const ObcTime* time);
step_t  OBCT_get_step(const ObcTime* time);
uint32_t OBCT_get_total_cycle_in_msec(const ObcTime* time);  // 計算上はstepも考慮
uint32_t OBCT_get_mode_cycle_in_msec(const ObcTime* time);   // 計算上はstepも考慮
float    OBCT_get_total_cycle_in_sec(const ObcTime* time);   // 計算上はstepも考慮（オーバーフローに注意）
float    OBCT_get_mode_cycle_in_sec(const ObcTime* time);    // 計算上はstepも考慮（オーバーフローに注意）
cycle_t OBCT_sec2cycle(uint32_t sec);
uint32_t OBCT_cycle2sec(cycle_t cycle);
ObcTime OBCT_diff(const ObcTime* before,
                  const ObcTime* after);
step_t OBCT_diff_in_step(const ObcTime* before,
                         const ObcTime* after);
uint32_t OBCT_diff_in_msec(const ObcTime* before,
                               const ObcTime* after);
float OBCT_diff_in_sec(const ObcTime* before,
                       const ObcTime* after);
void OBCT_print(const ObcTime* time);

/**
 * @brief OBCT_UnixTimeInfo 作成関数
 *
 *        観測した UnixTime とそれを観測した時の ObcTime を受け取り OBCT_UnixTimeInfo へ変える.
 * @param[in] time (GPS 等から) unix_time を観測した時の ObcTime
 * @param[in] unix_time (GPS 等から観測した) unix_time
 * @return OBCT_UnixTimeInfo 引数から計算した OBCT_UnixTimeInfo
 */
OBCT_UnixTimeInfo OBCT_create_unix_time_info(const double unix_time, const ObcTime* time);
/**
 * @brief OBCT_UnixTimeInfo クリア関数, 全て0にする
 * @param[in] uti クリアする OBCT_UnixTimeInfo
 */
void OBCT_clear_unix_time_info(OBCT_UnixTimeInfo* uti);
/**
 * @brief OBCT_UnixTimeInfo 修正関数
 * @param[in] uti 修正する OBCT_UnixTimeInfo
 * @param[in] time (GPS 等から) unix_time を観測した時の ObcTime
 * @param[in] unix_time (GPS 等から観測した) unix_time
 */
void OBCT_modify_unix_time_info(OBCT_UnixTimeInfo* uti, const double unix_time, const ObcTime time);

#endif
