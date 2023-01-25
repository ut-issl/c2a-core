/**
 * @file
 * @brief C2A が依存する libc 関数 memchr を自前実装し，c2a-core から提供することで，C2A の移植性を高める．
 *        これにより，ベアメタル環境でも C2A を libc 無しに（newlib などを持ち出してくることなく）ビルド・動作させることができる．
 * @note  https://github.com/ut-issl/c2a-core/pull/485
 * @note  https://linuxjm.osdn.jp/html/LDP_man-pages/man3/memchr.3.html
 */
#include <string.h>

void* memchr(const void* buf, int c, size_t n)
{
  const unsigned char* s = (const unsigned char*) buf;

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
