/**
 * @file
 * @brief  EH の Rule 共通ヘッダ
 */
#ifndef EVENT_HANDLER_RULES_H_
#define EVENT_HANDLER_RULES_H_

/**
 * @enum  AH_RULE_ID
 * @brief EH_Rule の ID
 * @note  最大数は EH_RULE_MAX で規定
 * @note  uint16_t を想定
 */
typedef enum
{
  EH_RULE_TEST0 = 100,
  EH_RULE_TEST1 = 101,
  EH_RULE_TEST2 = 102,
  EH_RULE_TEST3 = 103,
  EH_RULE_TEST4 = 104,
  EH_RULE_TEST5 = 105,
  EH_RULE_TEST6 = 106,
  EH_RULE_TEST7 = 107
} EH_RULE_ID;


/**
 * @brief  event_handler のデフォルトルールを読み込む
 * @param  void
 * @return void
 */
void EH_load_default_rules(void);

// 以下，ユーザー定義 ルール登録関数宣言
void EH_load_rule_test(void);

#endif
