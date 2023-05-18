#ifndef OBC_TIME_CONFIG_H_
#define OBC_TIME_CONFIG_H_

// step, cycleについてはTimeManagerを参照のこと
// デフォルト設定
#define OBCT_STEP_IN_MSEC (1)                                                    //!< 1 step で何 ms か
#define OBCT_STEPS_PER_CYCLE (100)                                               //!< 何 step で 1 cycle か
#define OBCT_CYCLES_PER_SEC (1000 / OBCT_STEP_IN_MSEC / OBCT_STEPS_PER_CYCLE)    //!< 1 s で何 cycle か
#define OBCT_MAX_CYCLE (0xfffffff0u)                                             //!< 最大 cycle 数．つまり TI がいくつでオーバーフローするか

// user 設定
#include <src_user/Settings/System/obc_time_params.h>

#endif // OBC_TIME_CONFIG_H_
