/**
 * @file git_revision.h
 * @brief git revisionをコードに埋め込む
 */
#ifndef GIT_REVISION_H_
#define GIT_REVISION_H_

#include <stdint.h>

#include <src_user/Library/git_revision.h> // config

#ifdef C2A_GIT_REVISION_IGNORE
// commit hashを埋め込まない場合

#define GIT_REVISION_C2A_CORE         "0000000000000000000000000000000000000000"
#define GIT_REVISION_C2A_CORE_SHORT   0x0000000
#define GIT_REVISION_C2A_USER         "0000000000000000000000000000000000000000"
#define GIT_REVISION_C2A_USER_SHORT   0x0000000

#else

// commit hashを埋め込むはずなのにsrc_user/Library/git_revision.hで設定されていない
#if !defined(C2A_GIT_REVISION_FROM_OPTION) && !defined(C2A_GIT_REVISION_GENERATED)
#error "please specify git revision config in src_user/Library/git_revision.h"
#endif // error

#endif // C2A_GIT_REVISION_IGNORE

extern const char GIT_REV_CORE[41];
extern const uint32_t GIT_REV_CORE_SHORT;
extern const char GIT_REV_USER[41];
extern const uint32_t GIT_REV_USER_SHORT;

#endif // GIT_REVISION_H_
