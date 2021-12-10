/**
 * @file git_revision.h
 * @brief git revisionをコードに埋め込む
 */
#ifndef GIT_REVISION_H_
#define GIT_REVISION_H_

#include <stdint.h>

#ifndef GIT_REVISION_C2A_CORE
  #define GIT_REVISION_C2A_CORE         "0000000000000000000000000000000000000000"
  #define GIT_REVISION_C2A_CORE_SHORT   0x0000000
#endif

#ifndef GIT_REVISION_C2A_USER
  #define GIT_REVISION_C2A_USER         "0000000000000000000000000000000000000000"
  #define GIT_REVISION_C2A_USER_SHORT   0x0000000
#endif

extern const char GIT_REV_CORE[41];
extern const uint32_t GIT_REV_CORE_SHORT;
extern const char GIT_REV_USER[41];
extern const uint32_t GIT_REV_USER_SHORT;

#endif // GIT_REVISION_H_
