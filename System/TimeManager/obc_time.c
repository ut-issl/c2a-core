#pragma section REPRO
#include "obc_time.h"

#include "../../Library/print.h"

ObcTime OBCT_create(cycle_t total_cycle,
                    cycle_t mode_cycle,
                    step_t step)
{
  ObcTime time;

  time.total_cycle = total_cycle;
  time.mode_cycle  = mode_cycle;
  time.step        = step;

  return time;
}

ObcTime OBCT_get_max(void)
{
  ObcTime max = {
    OBCT_MAX_CYCLE - 1,
    OBCT_MAX_CYCLE - 1,
    OBCT_STEPS_PER_CYCLE - 1
  };
  return max;
}

void OBCT_clear(ObcTime* time)
{
  time->total_cycle = 0;
  time->mode_cycle = 0;
  time->step = 0;
}

void OBCT_count_up(ObcTime* time)
{
  ++(time->step);

  if (time->step == OBCT_STEPS_PER_CYCLE)
  {
    ++(time->total_cycle);

    if (time->total_cycle == OBCT_MAX_CYCLE)
    {
      time->total_cycle = 0;
    }

    ++(time->mode_cycle);

    if (time->mode_cycle == OBCT_MAX_CYCLE)
    {
      time->mode_cycle = 0;
    }

    time->step = 0;
  }
}

cycle_t OBCT_get_total_cycle(const ObcTime* time)
{
  return (cycle_t)time->total_cycle;
}

cycle_t OBCT_get_mode_cycle(const ObcTime* time)
{
  return (cycle_t)time->mode_cycle;
}

step_t OBCT_get_step(const ObcTime* time)
{
  return (step_t)time->step;
}

uint32_t OBCT_get_total_cycle_in_msec(const ObcTime* time)
{
  return OBCT_STEP_IN_MSEC * (OBCT_STEPS_PER_CYCLE * time->total_cycle + time->step);
}

uint32_t OBCT_get_mode_cycle_in_msec(const ObcTime* time)
{
  return OBCT_STEP_IN_MSEC * (OBCT_STEPS_PER_CYCLE * time->mode_cycle + time->step);
}

float OBCT_get_total_cycle_in_sec(const ObcTime* time)
{
  float cycle_in_sec = (float)(time->total_cycle) / OBCT_CYCLES_PER_SEC;
  float step_in_sec = 0.001f * time->step * OBCT_STEP_IN_MSEC;
  return cycle_in_sec + step_in_sec;
}

float OBCT_get_mode_cycle_in_sec(const ObcTime* time)
{
  float cycle_in_sec = (float)(time->mode_cycle) / OBCT_CYCLES_PER_SEC;
  float step_in_sec = 0.001f * time->step * OBCT_STEP_IN_MSEC;
  return cycle_in_sec + step_in_sec;
}

cycle_t OBCT_sec2cycle(uint32_t sec)
{
  return (1000 * sec) / (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE);
}

uint32_t OBCT_cycle2sec(cycle_t cycle) {
  return (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE * cycle) / 1000;
}

cycle_t OBCT_msec2cycle(uint32_t msec)
{
  return (msec) / (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE);
}

uint32_t OBCT_cycle2msec(cycle_t cycle) {
  return (OBCT_STEP_IN_MSEC * OBCT_STEPS_PER_CYCLE * cycle);
}

ObcTime OBCT_diff(const ObcTime* before,
                  const ObcTime* after)
{
  ObcTime diff;

  // まずcycleの差分を計算
  diff.total_cycle = after->total_cycle - before->total_cycle;
  diff.mode_cycle = after->mode_cycle - before->mode_cycle;

  // stepのみで差分を考えればよい場合
  if (after->step >= before->step)
  {
    diff.step = after->step - before->step;
  }
  // cycleからの桁借りが必要な場合
  else
  {
    diff.step = OBCT_STEPS_PER_CYCLE - before->step + after->step;
    --diff.total_cycle;
    --diff.mode_cycle;
  }

  return diff;
}

ObcTime OBCT_add(const ObcTime* left, const ObcTime* right)
{
  ObcTime ret;

  ret.total_cycle = left->total_cycle + right->total_cycle;
  ret.mode_cycle = left->mode_cycle + right->mode_cycle;
  ret.step = left->step + right->step;

  ret.total_cycle += ret.step / OBCT_STEPS_PER_CYCLE;
  ret.mode_cycle += ret.step / OBCT_STEPS_PER_CYCLE;
  ret.step %= OBCT_STEPS_PER_CYCLE;

  return ret;
}

step_t OBCT_diff_in_step(const ObcTime* before,
                         const ObcTime* after)
{
  ObcTime diff = OBCT_diff(before, after);
  return OBCT_STEPS_PER_CYCLE * diff.total_cycle + diff.step;
}

uint32_t OBCT_diff_in_msec(const ObcTime* before,
                               const ObcTime* after)
{
  return OBCT_STEP_IN_MSEC * OBCT_diff_in_step(before, after);
}

float OBCT_diff_in_sec(const ObcTime* before,
                       const ObcTime* after)
{
  return 0.001f * OBCT_diff_in_msec(before, after);
}

int OBCT_compare(const ObcTime* t1, const ObcTime* t2)
{
  if (t1->total_cycle == t2->total_cycle)
  {
    if (t1->step == t2->step) return 0;
    return (t1->step < t2->step) ? 1 : -1;
  }

  return (t1->total_cycle < t2->total_cycle) ? 1 : -1;
}

void OBCT_print(const ObcTime* time)
{
  Printf("Time: <total, mode, step> = <%d, %d, %d>\n", time->total_cycle, time->mode_cycle, time->step);
}

OBCT_UnixTimeInfo OBCT_create_unix_time_info(const double unix_time, const ObcTime* time)
{
  OBCT_UnixTimeInfo uti;
  step_t diff_step = OBCT_STEPS_PER_CYCLE * time->total_cycle + time->step;
  uint32_t diff_msec = OBCT_STEP_IN_MSEC * diff_step;
  float diff_sec = 0.001f * diff_msec;

  uti.unix_time_on_ti0 = unix_time - diff_sec;
  uti.last_update_ti = time->total_cycle;

  return uti;
}

void OBCT_clear_unix_time_info(OBCT_UnixTimeInfo* uti)
{
  uti->unix_time_on_ti0 = 0.0;
  uti->last_update_ti = 0;
}

void OBCT_modify_unix_time_info(OBCT_UnixTimeInfo* uti, const double unix_time, const ObcTime time)
{
  *uti = OBCT_create_unix_time_info(unix_time, &time);
}

#pragma section
