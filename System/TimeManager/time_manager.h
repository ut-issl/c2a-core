#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

#include "obc_time.h"
#include "../../CmdTlm/common_tlm_cmd_packet.h"

typedef struct
{
  ObcTime initializing_time;
  uint8_t initializing_flag;
} TimeManager;

extern const TimeManager* const time_manager;

void TMGR_init(void);
void TMGR_clear(void);
void TMGR_clear_master_mode_cycle(void);
void TMGR_count_up_master_clock(void);
void TMGR_down_initializing_flag(void);

ObcTime TMGR_get_master_clock(void);
ObcTime TMGR_get_master_clock_from_boot(void);
cycle_t TMGR_get_master_total_cycle(void);
cycle_t TMGR_get_master_mode_cycle(void);
step_t  TMGR_get_master_step(void);
uint32_t TMGR_get_master_total_cycle_in_msec(void);  // 計算上はstepも考慮（オーバーフローに注意）
uint32_t TMGR_get_master_mode_cycle_in_msec(void);   // 計算上はstepも考慮（オーバーフローに注意）

OBCT_UnixtimeInfo TMGR_get_obct_unixtime_info(void);
double TMGR_get_unixtime_from_obc_time(const ObcTime* time);
ObcTime TMGR_get_obc_time_from_unixtime(const double unixtime);
cycle_t TMGR_get_c2a_unixtime_from_unixtime(const double unixtime);
cycle_t TMGR_get_ti_from_c2a_unixtime(const cycle_t c2a_unixtime);
void TMGR_update_unixtime_info(const double unixtime, const ObcTime time);

CCP_EXEC_STS Cmd_TMGR_SET_TIME(const CTCP* packet);
CCP_EXEC_STS Cmd_TMGR_SET_UNIXTIME(const CTCP* packet);

#endif
