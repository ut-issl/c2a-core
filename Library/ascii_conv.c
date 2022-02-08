#pragma section REPRO
#include "ascii_conv.h"

unsigned char ascii2hex(unsigned short ascii)
{
  unsigned char hex = 0x00;
  unsigned char* ascii_p = (unsigned char*)&ascii;

  if ((ascii_p[0] >= '0') && (ascii_p[0] <= '9'))
  {
    hex |= (unsigned char)(((ascii_p[0] - '0') & 0x0F) << 4); // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[0] - '0') & 0x0F) << 0); // リトルエンディアン用
  }
  else if ((ascii_p[0] >= 'A') && (ascii_p[0] <= 'F'))
  {
    hex |= (unsigned char)(((ascii_p[0] - 'A' + 10) & 0x0F) << 4);  // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[0] - 'A' + 10) & 0x0F) << 0);  // リトルエンディアン用
  }
  else if ((ascii_p[0] >= 'a') && (ascii_p[0] <= 'f'))
  {
    hex |= (unsigned char)(((ascii_p[0] - 'a' + 10) & 0x0F) << 4);  // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[0] - 'a' + 10) & 0x0F) << 0);  // リトルエンディアン用
  }

  if ((ascii_p[1] >= '0') && (ascii_p[1] <= '9'))
  {
    hex |= (unsigned char)(((ascii_p[1] - '0') & 0x0F) << 0); // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[1] - '0') & 0x0F) << 4); // リトルエンディアン用
  }
  else if ((ascii_p[1] >= 'A') && (ascii_p[1] <= 'F'))
  {
    hex |= (unsigned char)(((ascii_p[1] - 'A' + 10) & 0x0F) << 0);  // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[1] - 'A' + 10) & 0x0F) << 4);  // リトルエンディアン用
  }
  else if ((ascii_p[1] >= 'a') && (ascii_p[1] <= 'f'))
  {
    hex |= (unsigned char)(((ascii_p[1] - 'a' + 10) & 0x0F) << 0);  // ビッグエンディアン用
    // hex |= (unsigned char)(((ascii_p[1] - 'a' + 10) & 0x0F) << 4);  // リトルエンディアン用
  }

  return (hex);
}


unsigned short  hex2ascii(unsigned char hex)
{
  unsigned short  ascii = 0x0000;
  unsigned char* ascii_p = (unsigned char*)&ascii;

  if (((hex >> 4) & 0x0F) <= 0x09)
  {
    ascii_p[0] = (unsigned char)(((hex >> 4) & 0x0F) + '0');  // ビッグエンディアン用
    // ascii_p[1] = (unsigned char)(((hex >> 4) & 0x0F) + '0');  // リトルエンディアン用
  }
  else if (((hex >> 4) & 0x0F) >= 0x0A)
  {
    ascii_p[0] = (unsigned char)((((hex >> 4) & 0x0F) + 'A') - 10); // ビッグエンディアン用
    // ascii_p[1] = (unsigned char)((((hex >> 4) & 0x0F) + 'A') - 10); // リトルエンディアン用
  }

  if (((hex >> 0) & 0x0F) <= 0x09)
  {
    ascii_p[1] = (unsigned char)(((hex >> 0) & 0x0F) + '0');  // ビッグエンディアン用
    // ascii_p[0] = (unsigned char)(((hex >> 0) & 0x0F) + '0');  // リトルエンディアン用
  }
  else if (((hex >> 0) & 0x0F) >= 0x0A)
  {
    ascii_p[1] = (unsigned char)((((hex >> 0) & 0x0F) + 'A') - 10); // ビッグエンディアン用
    // ascii_p[0] = (unsigned char)((((hex >> 0) & 0x0F) + 'A') - 10); // リトルエンディアン用
  }

  return (ascii);
}
#pragma section
