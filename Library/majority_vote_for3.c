#pragma section REPRO
/**
 * @file
 * @brief  3重冗長用汎用多数決関数
 */
#include "majority_vote_for3.h"
#include <string.h>               // for memcpy;

MAJORITY_VOTE_FOR3_STATUS majority_vote_for3_copy(void* res_arg,
                                                  void* array1_arg,
                                                  void* array2_arg,
                                                  void* array3_arg,
                                                  size_t type_len,
                                                  uint32_t length)
{
  unsigned char* res    = (unsigned char*)res_arg;
  unsigned char* array1 = (unsigned char*)array1_arg;
  unsigned char* array2 = (unsigned char*)array2_arg;
  unsigned char* array3 = (unsigned char*)array3_arg;

  uint32_t i;
  MAJORITY_VOTE_FOR3_STATUS ret = MAJORITY_VOTE_MATCH;

  for (i = 0; i < length; ++i)
  {
    int flag12 = memcmp(array1, array2, type_len);
    int flag23 = memcmp(array2, array3, type_len);

    if (flag12 == 0 && flag23 == 0)
    {
      memcpy(res, array1, type_len);
    }
    else
    {
      int flag31 = memcmp(array3, array1, type_len);

      if (flag12 == 0 || flag23 == 0 || flag31 == 0)
      {
        ret = MAJORITY_VOTE_MAJORITY;
        if (flag12 != 0)
        {
          memcpy(res, array3, type_len);
        }
        else if (flag23 != 0)
        {
          memcpy(res, array1, type_len);
        }
        else
        {
          memcpy(res, array2, type_len);
        }
      }
      else
      {
        return MAJORITY_VOTE_ERROR; // 全不一致は即return
      }
    }

    res    += type_len;
    array1 += type_len;
    array2 += type_len;
    array3 += type_len;
  }

  return ret;
}

MAJORITY_VOTE_FOR3_STATUS majority_vote_for3_sync(void* array1_arg,
                                                  void* array2_arg,
                                                  void* array3_arg,
                                                  size_t type_len,
                                                  uint32_t length)
{
  unsigned char* array1 = (unsigned char*)array1_arg;
  unsigned char* array2 = (unsigned char*)array2_arg;
  unsigned char* array3 = (unsigned char*)array3_arg;

  uint32_t i;
  MAJORITY_VOTE_FOR3_STATUS ret = MAJORITY_VOTE_MATCH;

  for (i = 0; i < length; ++i)
  {
    int flag12 = memcmp(array1, array2, type_len);
    int flag23 = memcmp(array2, array3, type_len);

    if (!(flag12 == 0 && flag23 == 0))
    {
      int flag31 = memcmp(array3, array1, type_len);

      if (flag12 == 0 || flag23 == 0 || flag31 == 0)
      {
        ret = MAJORITY_VOTE_MAJORITY;

        if (flag12 == 0)
        {
          memcpy(array3, array1, type_len);
        }
        else if (flag23 == 0)
        {
          memcpy(array1, array2, type_len);
        }
        else
        {
          memcpy(array2, array3, type_len);
        }
      }
      else
      {
        return MAJORITY_VOTE_ERROR; // 全不一致は即return
      }
    }
  }

  return ret;
}

#pragma section
