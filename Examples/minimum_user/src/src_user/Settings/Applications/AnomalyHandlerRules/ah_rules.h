#ifndef AH_RULES_H_
#define AH_RULES_H_
/**
 * @file
 * @brief  AHのユーザー定義部分をCoreから分離
 */

// 現時点で，最大数は128個！
typedef enum
{
  AH_RULE_SAMPLE
} AH_RULE_NUM;

enum
{
  AL_RULE_INACTIVE = 0,
  AL_RULE_ACTIVE   = 1
};

void AH_Rule_default_load(void);

#endif
