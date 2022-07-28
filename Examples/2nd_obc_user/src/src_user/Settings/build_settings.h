#ifndef BUILD_SETTINGS_H_
#define BUILD_SETTINGS_H_

#include "sils_define.h"

#ifdef SILS_FW
#define IS_LITTLE_ENDIAN // SILS環境により変更する
#else
// #define IS_LITTLE_ENDIAN // マイコン環境により変更する
#endif

// #define BUILD_SETTINGS_FAST_BUILD    //!< 高速ビルドオプション（検証時は必ずコメントアウトすること）

#endif
