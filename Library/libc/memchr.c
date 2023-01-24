#include <string.h>

// https://linuxjm.osdn.jp/html/LDP_man-pages/man3/memchr.3.html

void *memchr(const void *buf, int c, size_t n)
{
  const unsigned char *s = (const unsigned char*) buf;

  while (n--)
  {
    if (*s == (unsigned char)c)
    {
      return (void*)s;
    }
    s++;
  }

  return NULL;
}
