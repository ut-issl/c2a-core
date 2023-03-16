/**
 * @file
 * @brief SPWIRE(スペースワイヤ)通信ラッパーのクラスです。
 *
 * SPWIRE(スペースワイヤ)通信ラッパーは、
 * SPWIREのインターフェースを実現し、
 * SPWIREポートの初期化、データ送信、データ受信を行う、
 * SPWIRE通信ラッパーのクラスです。
 * 個々の機器のインターフェースモジュールに継承?せさて使用します。
 * @note SPWIRE(スペースワイヤ)のルーティングは複数考えられますが、
 * 今回は接続相手機器がSOBC(EM)のCPUボード(J111〜J114ポート)同士で接続した場合の経路にのみ対応しています。
 * また、データのやりとりもCPUボード上のメモリマップ上にのみ限定されています(ディスティネーションキー固定:0x24)。
 * @warning J111,J112ポートはIFボード向けにAPIで使用されていたりするので、なるべく手を出さない方が良い。
 * また、今回J111ポートにはIFボードが接続されているはずなので、その設定で本クラスを使用しないはず。
 */

#ifndef SPWIRE_H_
#define SPWIRE_H_

#include <stdint.h>

/**
 * @class SPWIRE_Config
 * @brief SPWIREポートの初期化、データ送信、データ受信の際に必要となる設定情報を格納する構造体です。
 */
typedef struct
{
  uint8_t  own_port;   /**< @protected @brief 継承先の機器へつながっていく自身のポート番号の下1桁*/
  uint8_t  target_port;  /**< @protected @brief 継承先の機器のつながっているポート番号の下1桁*/
  uint32_t reg_addr;   /**< @protected @brief 継承先の機器の読み書きを行うレジスタアドレス(なるべく4の倍数アドレスを指定)*/
} SPWIRE_Config;


int SPWIRE_init(void* my_spwire_v);
int SPWIRE_rx(void* my_spwire_v, void* data_v, int buffer_size);
int SPWIRE_tx(void* my_spwire_v, void* data_v, int data_size);

#endif
