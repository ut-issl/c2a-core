#include <stdlib.h>

// https://linuxjm.osdn.jp/html/LDP_man-pages/man3/bsearch.3.html

// compare func(key, base[i])
// key < b: compr_func(key, b) < 0
// key = b: compr_func(key, b) = 0
// key > b: compr_func(key, b) > 0
typedef int (* compr_func)(const void *, const void *);

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, compr_func compr)
{
  size_t min = 0;
  size_t max = nmemb;

  if (nmemb == 0 || size == 0){
    return NULL;
  }

  while(min < max)
  {
    size_t index = (min + max) / 2;
    void *current = (void*) (base + (size * index));

    int result = compr(key, current);
    if (result == 0)
    {
      // found
      return current;
    }
    else if (result < 0)
    {
      // key < current
      max = index - 1;
    }
    else // result > 0
    {
      // current < key
      min = index + 1;
    }
  }

  // not found
  return NULL;
}
