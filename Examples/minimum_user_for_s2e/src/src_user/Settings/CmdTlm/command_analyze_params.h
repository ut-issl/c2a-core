/**
 * @file   command_analyze_params.h
 * @brief  CAのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef COMMAND_ANALYZE_PARAMS_H_
#define COMMAND_ANALYZE_PARAMS_H_

#undef CA_TLM_PAGE_SIZE
#undef CA_TLM_PAGE_MAX
#undef CMD_MAX_CMDS

// 1テレメトリパケット(=1ページ)に格納されるコマンド数
#define CA_TLM_PAGE_SIZE (64)
// コマンドテーブルページ数
#define CA_TLM_PAGE_MAX (24)
// コマンドテーブルサイズ
#define CMD_MAX_CMDS (CA_TLM_PAGE_SIZE * CA_TLM_PAGE_MAX)

#endif
