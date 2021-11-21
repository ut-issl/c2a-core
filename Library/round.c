#pragma section REPRO

/**
 * @file round.c
 * @brief  ŽlŽÌŒÜ“ü
 */
#include "round.h"

int round(double src)
{
  int dst;
  dst = (int)(src + 0.5);
  return dst;
}

#pragma section
