/**
 * @file
 * @brief PA(PassiveAnalog)ラッパーのクラスです。(MOBC用)
 *
 * PA(PassiveAnalog)ラッパーは、
 * PAのインターフェースを実現し、
 * PAポートの初期化、電流値取得を行う、
 * PAラッパーのクラスです。
 * 個々の機器のインターフェースモジュールに継承?せさて使用します。
 */

#ifndef PA_H_
#define PA_H_

#include <stdint.h>

/**
 * @class PA_Config
 * @brief PAポートの初期化、電流値取得の際に必要となる設定情報を格納する構造体です。
 */
typedef struct
{
  uint16_t  ch; /**< @protected @brief  チャネル指定のbit0がポート1に、bit15がポート16に対応し、1を設定したポートの電圧値だけを取得します。*/
} PA_Config;


int PA_init(void* my_pa_v);
int PA_rx(void* my_pa_v, void* data_v, int buffer_size);
int PA_tx(void* my_pa_v, void* data_v, int data_size);

#endif
