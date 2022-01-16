#ifndef SILS_DEFINE_H_
#define SILS_DEFINE_H_

#ifndef SILS_FW
// #define SILS_FW //ここでdefineする！
#endif // SILS_FW

#ifdef SILS_FW
#define IS_LITTLE_ENDIAN // SILS環境により変更する
#endif

#endif
