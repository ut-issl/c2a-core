/**
 * @file
 * @brief git revisionをコードに埋め込む
 */
#ifndef GIT_REVISION_H_
#define GIT_REVISION_H_

#include "stdint.h"

extern const char GIT_REV_CORE[41];
extern const uint32_t GIT_REV_CORE_SHORT;
extern const char GIT_REV_USER[41];
extern const uint32_t GIT_REV_USER_SHORT;

#endif // GIT_REVISION_H_
