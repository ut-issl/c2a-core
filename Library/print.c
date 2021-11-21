/**
 * @file print.c
 * @brief  HW依存Print
 */


// printはHW依存性が強いため，ヘッダーはcoreにおき，適宜userからオーバーライドする
// ビルド対象にて選択する
#include "print.h"

void Printf(char* format, ...)
{
}
