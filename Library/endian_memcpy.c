#pragma section REPRO
/**
 * @file
 * @brief  memcpyのエンディアン対策
 */

/*
memcpyでエンディアンのせいで死ぬことがあるので，
OBCではmemcpy，SLIS環境ではendian convが入るようにスイッチさせる．

memcpy(&sci_.sib_rev, CCP_get_param_head(packet), 2);
↓
endina_memcpy(&sci_.sib_rev, CCP_get_param_head(packet), 2);
*/

#include "endian_memcpy.h"
#include "endian_conv.h"
#include "string.h" // for memcpy
#include <src_user/Settings/build_settings.h>

#ifdef SILS_FW
#include "stdlib.h" // for malloc
#endif // SILS_FW

void* endian_memcpy(void* dest, const void* src, size_t count)
{
#ifdef IS_LITTLE_ENDIAN
  // 内部でcopyと同等のことを行っている
  endian_conv(src, dest, (int)count);
#else
  memcpy(dest, src, count);
#endif

  return dest;
}

#pragma section
