/**
 * @file
 * @brief エンディアンに関するものを定義する
 */
#pragma section REPRO
#include "endian.h"
#include <src_user/Settings/build_settings.h>
#include <stdint.h>
#include <string.h>

void* ENDIAN_memcpy(void* dest, const void* src, size_t size)
{
#ifdef IS_LITTLE_ENDIAN
  // 内部でcopyと同等のことを行っている
  ENDIAN_conv(dest, src, size);
#else
  memcpy(dest, src, size);
#endif

  return dest;
}

void ENDIAN_conv(void* after, const void* before, size_t size)
{
  const uint8_t* bef = (const uint8_t*)before;
  uint8_t* aft = (uint8_t*)after;
  size_t i;

  if (size < 0) return;

  size--;
  for (i = 0; i <= size; i++)
  {
    *(aft + (size - i)) = *(bef + i);
  }

  return;
}

#pragma section
