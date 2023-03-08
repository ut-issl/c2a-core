/**
 * @file
 * @brief AA(ActiveAnalog)ラッパーのクラスです。
 *
 * AA(ActiveAnalog)ラッパーは、
 * AAのインターフェースを実現し、
 * AAポートの初期化、電圧値取得を行う、
 * AAラッパーのクラスです。
 * 個々の機器のインターフェースモジュールに継承?せさて使用します。
 */

#ifndef AA_H_
#define AA_H_

#include <stdint.h>

/**
 * @class AA_Config
 * @brief AAポートの初期化、電圧値取得の際に必要となる設定情報を格納する構造体です。
 */
typedef struct
{
  uint8_t ch; /**< @protected @brief  チャネル指定のbit0がポート1に、bit7がポート8に対応し、1を設定したポートの電圧値だけを取得します。*/
} AA_Config;


int AA_init(void* my_aa_v);
int AA_rx(void* my_aa_v, void* data_v, int buffer_size);
int AA_tx(void* my_aa_v, void* data_v, int data_size);

#endif
