#pragma section REPRO
#include "time_manager.h"
#include <string.h>
#include <src_user/CmdTlm/Ccsds/TCPacket.h>
#include "../TaskManager/task_dispatcher.h"
#include "../../Library/endian_memcpy.h"
#include "../../CmdTlm/common_tlm_cmd_packet_util.h"

static TimeManager time_manager_;
const TimeManager* const time_manager = &time_manager_;

static void TMGR_set_master_total_cycle_(cycle_t total_cycle);

void TMGR_init(void)
{
  OBCT_clear(&time_manager_.init_info_.initializing_time);
  time_manager_.init_info_.initializing_flag = 1;
  TMGR_clear();
}

void TMGR_clear(void)
{
  OBCT_clear(&time_manager_.master_clock_);
  OBCT_clear_unixtime_info(&time_manager_.unixtime_info_);
}

void TMGR_clear_master_mode_cycle(void)
{
  time_manager_.master_clock_.mode_cycle = 0;
}

#pragma section _FIX_TMGR
// Pセクションしか割り当てていないので，
// D, Bセクションに乗るような変数定義は禁止！！！
void TMGR_count_up_master_clock(void)
{
  OBCT_count_up(&time_manager_.master_clock_);
}
#pragma section
#pragma section REPRO

void TMGR_down_initializing_flag(void)
{
  memcpy(&time_manager_.init_info_.initializing_time, &time_manager_.master_clock_, sizeof(ObcTime));
  time_manager_.init_info_.initializing_flag = 0;

  TMGR_clear();
}

ObcTime TMGR_get_master_clock(void)
{
  if (time_manager_.init_info_.initializing_flag)
  {
    return OBCT_create(0, 0, 0);
  }
  else
  {
    return time_manager_.master_clock_;
  }
}

ObcTime TMGR_get_master_clock_from_boot(void)
{
  return OBCT_add(&time_manager_.init_info_.initializing_time, &time_manager_.master_clock_);
}

cycle_t TMGR_get_master_total_cycle(void) {
  return OBCT_get_total_cycle(&time_manager_.master_clock_);
}

cycle_t TMGR_get_master_mode_cycle(void) {
  return OBCT_get_mode_cycle(&time_manager_.master_clock_);
}

step_t  TMGR_get_master_step(void) {
  return OBCT_get_step(&time_manager_.master_clock_);
}

uint32_t TMGR_get_master_total_cycle_in_msec(void)
{
  return OBCT_get_total_cycle_in_msec(&time_manager_.master_clock_);
}

uint32_t TMGR_get_master_mode_cycle_in_msec(void)
{
  return OBCT_get_mode_cycle_in_msec(&time_manager_.master_clock_);
}

static void TMGR_set_master_total_cycle_(cycle_t total_cycle)
{
  time_manager_.master_clock_.total_cycle = total_cycle;
}

OBCT_UnixtimeInfo TMGR_get_obct_unixtime_info(void)
{
  return time_manager_.unixtime_info_;
}

double TMGR_get_unixtime_from_obc_time(const ObcTime* time)
{
  ObcTime ti0 = OBCT_create(0, 0, 0);
  return time_manager_.unixtime_info_.unixtime_at_ti0 + OBCT_diff_in_sec(&ti0, time);
}

ObcTime TMGR_get_obc_time_from_unixtime(const double unixtime)
{
  double diff_double = unixtime - time_manager_.unixtime_info_.unixtime_at_ti0;
  ObcTime res;
  uint32_t diff;
  cycle_t cycle_diff;
  step_t step_diff;

  if (diff_double < 0)  // あり得ない, おかしい
  {
    return res = OBCT_create(0, 0, 0);
  }

  diff = (uint32_t)(diff_double * 1000.0 + 1e-4); // msオーダーだがそんなに大きくないことを想定, 1e-4は数値誤差対策（.999がj切り捨てられるのを防ぐ）
  cycle_diff = diff / (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE);
  step_diff = (diff - cycle_diff * (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE)) / OBCT_STEP_IN_MSEC;

  res.total_cycle = cycle_diff;
  res.mode_cycle = 0; // 取得出来ないので0とする
  res.step = step_diff;

  return res;
}

cycle_t TMGR_get_utl_unixtime_from_unixtime(const double unixtime)
{
  return OBCT_get_utl_unixtime_from_unixtime(unixtime);
}

cycle_t TMGR_get_ti_from_utl_unixtime(const cycle_t utl_unixtime)
{
  cycle_t utl_unixtime_at_ti0 = TMGR_get_utl_unixtime_from_unixtime(time_manager_.unixtime_info_.unixtime_at_ti0);
  return utl_unixtime - utl_unixtime_at_ti0;
}

void TMGR_update_unixtime_info(const double unixtime, const ObcTime time)
{
  OBCT_update_unixtime_info(&time_manager_.unixtime_info_, unixtime, time);
}


CCP_EXEC_STS Cmd_TMGR_SET_TIME(const CTCP* packet)
{
  cycle_t set_value = CCP_get_param_from_packet(packet, 0, cycle_t);

  if (set_value < OBCT_MAX_CYCLE)
  {
    TMGR_set_master_total_cycle_(set_value);
    TDSP_resync_internal_counter();
    return CCP_EXEC_SUCCESS;
  }
  else
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
}

CCP_EXEC_STS Cmd_TMGR_SET_UNIXTIME(const CTCP* packet)
{
  ObcTime time;

  double unixtime = CCP_get_param_from_packet(packet, 0, double);
  time.total_cycle = CCP_get_param_from_packet(packet, 1, cycle_t);
  time.step = CCP_get_param_from_packet(packet, 2, step_t);
  time.mode_cycle = 0; // 必要ないので0とする

  TMGR_update_unixtime_info(unixtime, time);

  return CCP_EXEC_SUCCESS;
}

#pragma section
