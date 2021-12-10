#pragma section REPRO
/**
 * @file git_revision.c
 * @brief git revisionをコードに埋め込む
 */

#include "git_revision.h"

const char GIT_REV_CORE[41]         = GIT_REVISION_C2A_CORE;
const uint32_t GIT_REV_CORE_SHORT   = GIT_REVISION_C2A_CORE_SHORT;
const char GIT_REV_USER[41]         = GIT_REVISION_C2A_USER;
const uint32_t GIT_REV_USER_SHORT   = GIT_REVISION_C2A_USER_SHORT;

#pragma section
