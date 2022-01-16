#pragma section REPRO
/**
 * @file
 * @brief OBCの時刻情報を TimeManager 構造体に保持しカウントアップする. その他, 時刻演算に必要な関数も実装する
 */
#include "time_manager.h"
#include <string.h>
#include <src_user/TlmCmd/Ccsds/TCPacket.h>
#include "../TaskManager/task_dispatcher.h"
#include "../../TlmCmd/common_tlm_cmd_packet_util.h"

static TimeManager time_manager_;
const TimeManager* const time_manager = &time_manager_;

/**
 * @brief TI (master_clock_.total_cycle) の setter
 * @param[in] total_cycle
 * @return void
 */
static void TMGR_set_master_total_cycle_(cycle_t total_cycle);

/**
 * @brief utl_unixtime_epoch_ の setter
 * @param[in] utl_unixtime_epoch
 * @return void
 */
static void TMGR_set_utl_unixtime_epoch_(double utl_unixtime_epoch);

void TMGR_init(void)
{
  OBCT_clear(&time_manager_.init_info_.initializing_time);
  time_manager_.init_info_.initializing_flag = 1;
  TMGR_clear();
  TMGR_set_utl_unixtime_epoch_(TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL);
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

static void TMGR_set_utl_unixtime_epoch_(double utl_unixtime_epoch)
{
  time_manager_.utl_unixtime_epoch_ = utl_unixtime_epoch;
}

OBCT_UnixtimeInfo TMGR_get_obct_unixtime_info(void)
{
  return time_manager_.unixtime_info_;
}

double TMGR_get_unixtime_from_obc_time(const ObcTime* time)
{
  return TMGR_get_obct_unixtime_info().unixtime_at_ti0 + OBCT_get_total_cycle_in_sec(time);
}

ObcTime TMGR_get_obc_time_from_unixtime(const double unixtime)
{
  double diff = unixtime - TMGR_get_obct_unixtime_info().unixtime_at_ti0; // 秒単位
  cycle_t cycle_diff;
  step_t step_diff;
  ObcTime res;

  if (diff < 0)  // あり得ない, おかしい
  {
    return OBCT_create(0, 0, 0);
  }

  cycle_diff = (cycle_t)(diff * OBCT_CYCLES_PER_SEC); // cycle未満は切り捨て
  step_diff = (step_t)((diff * OBCT_CYCLES_PER_SEC  - cycle_diff) * OBCT_STEPS_PER_CYCLE); // step未満は切り捨て

  res.total_cycle = cycle_diff;
  res.mode_cycle = 0; // 取得出来ないので0とする
  res.step = step_diff;

  return res;
}

cycle_t TMGR_get_utl_unixtime_from_unixtime(const double unixtime)
{
  // 紀元より昔なのはおかしい
  if (unixtime < time_manager_.utl_unixtime_epoch_) return 0;

  // cycle 未満は切り捨て
  return (cycle_t)((unixtime - time_manager_.utl_unixtime_epoch_) * OBCT_CYCLES_PER_SEC);
}

cycle_t TMGR_get_ti_from_utl_unixtime(const cycle_t utl_unixtime)
{
  cycle_t utl_unixtime_at_ti0 = TMGR_get_utl_unixtime_from_unixtime(time_manager_.unixtime_info_.unixtime_at_ti0);
  return utl_unixtime - utl_unixtime_at_ti0;
}

void TMGR_update_unixtime_info(const double unixtime, const ObcTime* time)
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

  TMGR_update_unixtime_info(unixtime, &time);

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH(const CTCP* packet)
{
  double utl_unixtime_epoch = CCP_get_param_from_packet(packet, 0, double);
  TMGR_set_utl_unixtime_epoch_(utl_unixtime_epoch);

  return CCP_EXEC_SUCCESS;
}

#pragma section
