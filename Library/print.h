#ifndef PRINT_H_
#define PRINT_H_
/**
 * @file
 * @brief  HW 依存 Print, つまり ログ情報のフォーマット出力（デバッグ出力）
 */

// #include "../Settings/sils_define.h" //ここでincludeしておくと，Printf系のものは全てOKになる

// かなり危険な定義なので，一旦コメントアウト 2020/08/22
// typedef unsigned  char  u_char;
// typedef unsigned  short u_short;
// typedef unsigned  int   u_int;
// #define Byte(Address) *(volatile u_char*)Address
// #define Word(Address) *(volatile u_short *)Address
// #define Long(Address) *(volatile u_int*)Address

// /* ASCII Define */
// #define NUL   0
// #define STX   0x02
// #define ETX   0x03
// #define EOT   0x04
// #define ENQ   0x05
// #define ACK   0x06
// #define BS    0x08
// #define HT    0x09
// #define LF    0x0A
// #define CR    0x0D
// #define NAK   0x15
// #define SYN   0x16
// #define ETB   0x17
// #define SUB   0x1A
// #define ESC   0x1B
// #define SP    0x20

// #define RIGHT 0xCD
// #define LEFT  0xCB
// #define UP    0xC8
// #define DOWN  0xD0
// #define DEL   0xD3
// #define INS   0xD2

// #define ESC_SEQ   0x5B
// #define ESC_RIGHT 0x43
// #define ESC_LEFT  0x44
// #define ESC_UP    0x41
// #define ESC_DOWN  0x42

// #define NG    -1
// #define OK    0

void Printf(const char* format, ...);

#endif
