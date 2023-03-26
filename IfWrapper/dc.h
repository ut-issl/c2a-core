/**
 * @file
 * @brief  DC (Discrete, GPIO) ラッパーのクラス
 *
 *         DriverSuper Class は
 *         DCのインターフェースを実現し，
 *          DCポートの初期化，DC信号入出力を行う．
 */
#ifndef DC_H_
#define DC_H_

#include <stdint.h>

/**
 * @class DC_Config
 * @brief DCポートの初期化、DC信号入出力の際に必要となる設定情報を格納する構造体です。
 */
typedef struct
{
  uint16_t  ch_in;  /**< @protected @brief  チャネル指定のbit0がポート1に、bit15がポート16に対応し、1を設定したポートのDC信号だけを取得します。*/
  uint16_t  ch_out; /**< @protected @brief  チャネル指定のbit0がポート1に、bit15がポート16に対応し、1を設定したポートのDC信号だけを出力します。*/
} DC_Config;


/**
 * @brief  DCの初期化
 *
 *         DC_Config構造体をグローバルインスタンスとして定義し，ポインタを渡すことでポートを初期化する．
 * @param  *my_dc_v : 初期化するDC_Config構造体
 * @retval 0     : 正常終了
 * @retval 0以外 : エラー
 * @note   この関数を実行する前にDC_Config構造体内の設定値を設定しておく必要がある
 */
int DC_init(void* my_dc_v);

/**
 * @brief  DCの読み取り
 * @param  *my_dc_v    : DC_Config構造体
 * @param  *data_v     : DC信号値格納先へのポインタ
 * @param  buffer_size : DC信号値格納先のデータサイズ
 * @retval 0     : 受信データなし
 * @retval 1以上 : 受信データバイト数
 * @retval -1    : エラー
 */
int DC_rx(void* my_dc_v, void* data_v, int buffer_size);

/**
 * @brief  DCの出力設定
 * @param  *my_dc_v  : DC_Config構造体
 * @param  *data_v   : DC信号値格納先へのポインタ
 * @param  data_size : DC信号値格納先のデータサイズ
 * @retval 0     : 正常終了
 * @retval -1    : エラー
 */
int DC_tx(void* my_dc_v, void* data_v, int data_size);

// 追加 on 2019/04/21
// 公開関数
// int (*IF_reopen[])(void* my_if, int reason); に登録
int DC_reopen_dummy(void* my_dc_v, int reason);

#endif
