/**
 * @file
 * @brief  TFのCoreTlmパラメタのオーバーライド用ヘッダー
 */
#ifndef TELEMETRY_FRAME_PARAMS_H_
#define TELEMETRY_FRAME_PARAMS_H_

#undef TF_TLM_PAGE_SIZE
#undef TF_TLM_PAGE_MAX
#undef TF_MAX_TLMS

// 1テレメトリパケット(=1ページ)に格納されるテレメトリ数
#define TF_TLM_PAGE_SIZE (64)
// テレメトリテーブルページ数
#define TF_TLM_PAGE_MAX (4)
// コマンドテーブルサイズ
#define TF_MAX_TLMS (TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX)

#endif
