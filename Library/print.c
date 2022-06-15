/**
 * @file
 * @brief  HW 依存 Print, つまり ログ情報のフォーマット出力（デバッグ出力）
 */
// printはHW依存性が強いため，ヘッダーはcoreにおき，適宜userからオーバーライドする
// ビルド対象にて選択する
#include "print.h"

void Printf(const char* format, ...)
{
}
