#ifndef STDINT_H_
#define STDINT_H_

#include "../Settings/sils_define.h"

// HEWや外部ライブラリヘッダーとの重複typedefをガード
#ifndef _TYPEDEFINE_H_

#ifndef SILS_FW
// HEWに標準ヘッダstdint.hに相当するヘッダがなかったため、必要機能を自前で定義。
// bit幅指定の整数型をひとまず定義

typedef signed char    int8_t;
typedef signed short   int16_t;
typedef signed long    int32_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

#else
#include <stdint.h>
#endif // SILS_FW

#endif // _TYPEDEFINE_H_
#endif // STDINT_H_
