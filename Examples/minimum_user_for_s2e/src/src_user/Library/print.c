/**
 * @file
 * @brief  HW依存Print
 */

// こいつもBootLoaderに配置するので，リプロ可能領域のものをincludeしないこと！！

// printはHW依存性が強いため，ヘッダーはcoreにおき，オーバーライドする
// ビルド対象にて選択する
#include <src_core/Library/print.h>

#include <stdio.h>
#include <stdarg.h>

#include "../Settings/sils_define.h"

#ifndef SILS_FW
#include <src_core/System/WatchdogTimer/watchdog_timer.h>


/*
 *  ログ情報のフォーマット出力（デバッグ出力）
 */
char printf_buffer[512];

void Printf(char* format, ...)
{
  va_list argptr;

  WDT_clear_wdt();         // 2019/03/10 追加

  va_start(argptr, format);
  vsprintf(printf_buffer, format, argptr);

  tt_printf("%s", printf_buffer);
  // Printf_org("%s", printf_buffer);  // LVTTL UART ch1での出力．邪魔なので初期化とともに無効化 (2019-04-09)
  va_end(argptr);

  WDT_clear_wdt();         // 2019/03/10 追加
}

#else

void Printf(char* format, ...)
{
#ifdef SHOW_DEBUG_PRINT_ON_SILS
  char printf_buffer[512];   // バッファサイズよりでかい文字列が来ると死ぬ（が，これは実機環境でも同じか）
  va_list args;
  va_start(args, format);
  vsprintf(printf_buffer, format, args);

  printf("%s", printf_buffer);
  fflush(stdout);

  va_end(args);
#else
  // なにも表示しない
#endif
}

#endif
