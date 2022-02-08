/**
 * @file
 * @brief モード定義とモード遷移定義
 */
#ifndef MODE_DEFINITIONS_H_
#define MODE_DEFINITIONS_H_

typedef enum
{
  MD_MODEID_START_UP          = 0,          //  0: 起動直後
  MD_MODEID_INITIAL           = 1,          //  1: 起動後、及びモード切り替えの始点
  MD_MODEID_RESERVED_1        = 14,         // 14: 自由に使用できるモード
  MD_MODEID_RESERVED_2        = 15,         // 15: 自由に使用できるモード
  MD_MODEID_MODE_MAX                        //
} MD_MODEID;

void MD_load_mode_list(void);
void MD_load_transition_table(void);

#endif
