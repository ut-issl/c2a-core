#pragma section REPRO
#include "sum.h"

unsigned char sum(unsigned char* adr, unsigned int len)
{
  int i;
  unsigned char tmp;

  tmp = *adr;
  adr++;

  for (i = 1; i < len; i++)
  {
    tmp += *adr;
    adr++;
  }

  return (tmp);
}
#pragma section
