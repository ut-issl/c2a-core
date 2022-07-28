#include <src_core/IfWrapper/wdt.h>

int WDT_initialize(void* wdt_config)
{
  WDT_Config* my_wdt = (WDT_Config*)wdt_config;
  WDT_set_timer(wdt_config, 7);
  my_wdt->is_clear_enable = 1;
  my_wdt->is_wdt_enable = 1;

  return 0;
}

int WDT_clear(void* wdt_config)
{
  return 0;
}

int WDT_enable(void* wdt_config)
{
  return 0;
}

int WDT_disable(void* wdt_config)
{
  return 0;
}

int WDT_set_timer(void* wdt_config, int time)
{
  WDT_Config* my_wdt = (WDT_Config*)wdt_config;
  my_wdt->timer_setting = time;
  return 0;
}
