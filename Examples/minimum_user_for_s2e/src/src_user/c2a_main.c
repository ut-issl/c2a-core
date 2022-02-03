#include <src_core/Library/git_revision.h>
#include <src_core/Library/print.h>
#include <src_core/c2a_core_main.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/System/WatchdogTimer/watchdog_timer.h>
#include "./Settings/sils_define.h"

int main(void);
static void address_fixed_main_(void);
static void C2A_init_(void);
static void C2A_main_(void);
static void timer_setting_(void);

int main(void)
{
  address_fixed_main_();
  return 0;
}


// RAM上でのアドレス固定main関数
#pragma section _FIX_MAIN
// Pセクションしか割り当てていないので，
// D, Bセクションに乗るような変数定義は禁止！！！
static void address_fixed_main_(void)
{
#ifndef SILS_FW
  // BOOT_LOADER_main();
#endif
  C2A_init_();
  C2A_main_();
}
#pragma section


#pragma section REPRO
static void C2A_main_(void)
{
  while (1)       // whileもuserから隠蔽したいが，自由度とのトレードオフ．．．？
  {
    C2A_core_main();

    // ユーザー定義loop処理はここに入れる
  }

  // ここに来ることはないはずなので，来たらWDTのResetをかけるようにする？
}
#pragma section


#pragma section REPRO
// C2A関連の初期化
// HW関連部分（タイマー，割り込み設定など）のみ独自に初期化
// Printf内部で WDT_clear_wdt(); が呼ばれてることに注意！
static void C2A_init_(void)
{
  Printf("C2A_init: user revision = %s\n", GIT_REV_USER);

  WDT_init();

  TMGR_init();                // Time Manager
                              // AM_initialize_all_apps での時間計測のためにここで初期化

  Printf("C2A_init: TMGR_init done.\n");
  timer_setting_();           // Timer 割り込み開始
  Printf("C2A_init: timer_setting_ done.\n");

  C2A_core_init();

  // TaskDispatcherでの大量のアノマリを避けるために、一度時刻を初期化する。
  TMGR_clear();
  Printf("C2A_init: TMGR_init done.\n");
}
#pragma section


static void timer_setting_(void)
{
}
