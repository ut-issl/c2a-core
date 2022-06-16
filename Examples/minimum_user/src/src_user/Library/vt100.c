#pragma section REPRO
#include "vt100.h"

#include <src_core/Library/print.h>

void VT100_erase_down(void)
{
  Printf("\x1b[J");
}

void VT100_erase_up(void)
{
  Printf("\x1b[1J");
}

void VT100_erase_screen(void)
{
  Printf("\x1b[2J");
}

void VT100_erase_end_of_line(void)
{
  Printf("\x1b[K");
}

void VT100_erase_start_of_line(void)
{
  Printf("\x1b[1K");
}

void VT100_erase_line(void)
{
  Printf("\x1b[2K");
}

void VT100_set_color(VT100_ColorMode color)
{
  switch (color)
  {
  case RED:
    Printf("\x1b[31m");
    break;

  case GREEN:
    Printf("\x1b[32m");
    break;

  case YELLOW:
    Printf("\x1b[33m");
    break;

  case BLUE:
    Printf("\x1b[34m");
    break;

  default:
    Printf("\x1b[m");
    break;
  }
}

void VT100_set_background(VT100_ColorMode color)
{
  switch (color)
  {
  case RED:
    Printf("\x1b[41m");
    break;

  case GREEN:
    Printf("\x1b[42m");
    break;

  case YELLOW:
    Printf("\x1b[43m");
    break;

  case BLUE:
    Printf("\x1b[44m");
    break;

  default:
    Printf("\x1b[m");
    break;
  }
}

void VT100_reset_color(void)
{
  Printf("\x1b[m");
}

void VT100_reset_cursor(void)
{
  Printf("\x1B[0;0H");
}

#pragma section
