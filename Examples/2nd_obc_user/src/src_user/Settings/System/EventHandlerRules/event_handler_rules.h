/**
 * @file
 * @brief  EH の Rule 共通ヘッダ
 */
#ifndef EVENT_HANDLER_RULES_H_
#define EVENT_HANDLER_RULES_H_

/**
 * @enum  EH_RULE_ID
 * @brief EH_Rule の ID
 * @note  最大数は EH_RULE_MAX で規定
 * @note  uint16_t を想定
 */
typedef enum
{
  EH_RULE_TEST0 = 100
} EH_RULE_ID;


/**
 * @brief  event_handler のデフォルトルールを読み込む
 * @param  void
 * @return void
 */
void EH_load_default_rules(void);

#endif
