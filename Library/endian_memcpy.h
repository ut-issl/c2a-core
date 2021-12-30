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

#ifndef ENDIAN_MEMCPY_INCLUDE_
#define ENDIAN_MEMCPY_INCLUDE_


#include <stddef.h> // for size_t

// #pragma inline  endian_memcpy
void* endian_memcpy(void* dest, const void* src, size_t count);

#endif // ENDIAN_MEMCPY_INCLUDE_
