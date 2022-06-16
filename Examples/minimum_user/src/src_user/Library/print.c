/**
 * @file
 * @brief  HW 依存 Print, つまり ログ情報のフォーマット出力（デバッグ出力）
 */
// こいつもBootLoaderに配置するので，リプロ可能領域のものをincludeしないこと！！

// printはHW依存性が強いため，ヘッダーはcoreにおき，オーバーライドする
// ビルド対象にて選択する
#include <src_core/Library/print.h>

#include <stdio.h>
#include <stdarg.h>

#include "../Settings/sils_define.h"

// バッファサイズよりでかい文字列が来ると死ぬ
static char PRINT_buffer_[512];

#ifndef SILS_FW

#include <src_core/System/WatchdogTimer/watchdog_timer.h>

void Printf(const char* format, ...)
{
  va_list argptr;

  WDT_clear_wdt();         // 2019/03/10 追加

  va_start(argptr, format);
  vsprintf(PRINT_buffer_, format, argptr);

  tt_printf("%s", PRINT_buffer_);
  // Printf_org("%s", PRINT_buffer_);  // LVTTL UART ch1での出力．邪魔なので初期化とともに無効化 (2019-04-09)
  va_end(argptr);

  WDT_clear_wdt();         // 2019/03/10 追加
}

#else

void Printf(const char* format, ...)
{
#ifdef SHOW_DEBUG_PRINT_ON_SILS
  va_list args;
  va_start(args, format);
  vsprintf(PRINT_buffer_, format, args);

  printf("%s", PRINT_buffer_);
  fflush(stdout);

  va_end(args);
#else
  // なにも表示しない
#endif
}

#endif
