/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#ifndef BLOCK_COMMAND_DEFINITIONS_H_
#define BLOCK_COMMAND_DEFINITIONS_H_

// 登録されるBlockCommandTableのblock番号を規定
typedef enum
{

  // Block Cmds for Mode Transition (シーケンスリスト)
  // ./src_user/Settings/Modes/Transitions/ で定義
  BC_SL_START_UP_TO_INITIAL = 0,
  BC_SL_NOP  = 17,

  // Block Cmds for TaskList (タスクリスト) : 286-300
  // ./src_user/Settings/Modes/TaskLists/ で定義
  BC_TL_START_UP = 20,
  BC_TL_INITIAL = 21,

  // Block Cmds for Composition (App Rotator, Combinar)
  // ./src_user/Settings/Modes/TaskLists/Composition/ で定義
  BC_AR_DEBUG_DISPLAY_INI = 40,
  BC_AR_DRIVERS_UPDATE_INI = 42,
  BC_AR_GS_RELATED_PROCESS = 44,
  BC_AC_TLM_CMD_HIRATE = 50,

  // ==== 各系領域 ====
  // ./C2A/TlmCmd/NormalBlockCommandDefinition/で定義
  // アノマリハンドラはBC_AH_など，接頭辞を適切につけること！

  // CDH:60-79
  BC_HK_CYCLIC_TLM = 60,
  BC_RESERVED_FOR_HK = 77,    // EM電気試験でのコマンドファイルとのバッティングを防ぐ

  // COMM:90-99

  // ==== 地上からupする“のみ”領域 ====
  // C2Aでは使用しない

  // ==== 追加領域 ====

  // Telemetry Manager
  BC_TLM_MGR0 = 350,
  BC_TLM_MGR1 = 351,
  BC_TLM_MGR2 = 352,
  BC_TLM_MGR3 = 353,
  BC_TLM_MGR4 = 354,
  BC_TLM_MGR5 = 355,
  BC_TLM_MGR6 = 356,
  BC_TLM_MGR7 = 357,
  BC_TLM_MGR8 = 358,
  BC_TLM_MGR9 = 359,

  // Test
  BC_TEST_EH_RESPOND = 360,

  // BCT MAX : 382

  BC_ID_MAX    // BCT 自体のサイズは BCT_MAX_BLOCKS で規定
} BC_DEFAULT_ID;

void BC_load_defaults(void);

#endif
