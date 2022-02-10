#ifndef VT100_H_
#define VT100_H_

typedef enum
{
  RED,
  GREEN,
  YELLOW,
  BLUE
} VT100_ColorMode;

void VT100_erase_down(void);
void VT100_erase_up(void);
void VT100_erase_screen(void);
void VT100_erase_end_of_line(void);
void VT100_erase_start_of_line(void);
void VT100_erase_line(void);
void VT100_set_color(VT100_ColorMode color);
void VT100_reset_color(void);
void VT100_reset_cursor(void);

#endif
