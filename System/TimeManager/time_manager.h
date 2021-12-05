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
void TMGR_down_initializing_flag(void);
void TMGR_clear_master_mode_cycle(void);
void TMGR_count_up_master_clock(void);
uint32_t TMGR_get_master_total_cycle_in_msec(void);  // 計算上はstepも考慮（オーバーフローに注意）
uint32_t TMGR_get_master_mode_cycle_in_msec(void);   // 計算上はstepも考慮（オーバーフローに注意）
ObcTime TMGR_get_master_clock(void);
cycle_t TMGR_get_master_total_cycle(void);
cycle_t TMGR_get_master_mode_cycle(void);
step_t  TMGR_get_master_step(void);
double TMGR_get_unix_time_from_ObcTime(const ObcTime* time);
ObcTime TMGR_get_ObcTime_from_unix_time(const double unix_time);
void TMGR_modify_unix_time_criteria(const double unix_time, const ObcTime time);
OBCT_UnixTimeInfo TMGR_get_obct_unix_time_info(void);
ObcTime TMGR_get_master_clock_from_boot(void);

CCP_EXEC_STS Cmd_TMGR_SET_TIME(const CTCP* packet);
CCP_EXEC_STS Cmd_TMGR_SET_UNIXTIME(const CTCP* packet);

#endif
