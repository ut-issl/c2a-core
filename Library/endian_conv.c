#pragma section REPRO
#include "endian_conv.h"

int endian_conv(const void* bef_t, void* aft_t, int size)
{
  const unsigned char* bef = (const unsigned char*)bef_t;
  unsigned char* aft = (unsigned char*)aft_t;
  int       i;

  if (size < 0)
  {
    return -1;
  }

  size--;

  for (i = 0; i <= size; i++)
  {
    *(aft + (size - i)) = *(bef + i);
  }

  return 0;
}
#pragma section
