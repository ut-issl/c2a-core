/**
 * @file
 * @brief  CAのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef COMMAND_ANALYZE_PARAMS_H_
#define COMMAND_ANALYZE_PARAMS_H_

#undef CA_TLM_PAGE_SIZE
#undef CA_TLM_PAGE_MAX
#undef CA_MAX_CMDS

// 1テレメトリパケット(=1ページ)に格納されるコマンド数
#define CA_TLM_PAGE_SIZE (32)
// コマンドテーブルページ数
#define CA_TLM_PAGE_MAX (48)
// コマンドテーブルサイズ
#define CA_MAX_CMDS (CA_TLM_PAGE_SIZE * CA_TLM_PAGE_MAX)

#endif
