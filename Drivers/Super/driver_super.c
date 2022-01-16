#pragma section REPRO
/**
 * @file
 * @brief  各制御センサ・アクチュエータ等とのインターフェースドライバ群のスーパークラス
 *
 *         DriverSuper Class は
 *         各制御センサ・アクチュエータ等とのインターフェースを実現し，
 *         初期化，コマンド発行，テレメトリリクエスト，テレメトリ受信，テレメトリ解析などを行う，ドライバ群のスーパークラスです．
 *         個々の機器のインターフェースドライバに継承させて使用します．
 */

#include "driver_super.h"
#include "../../Library/print.h"
#include <string.h>     // for memsetなどのmem系
#include <stddef.h>     // for NULL

// #define DS_DEBUG                       // 適切なときにコメントアウトする
// #define DS_DEBUG_SHOW_REC_DATA         // 適切なときにコメントアウトする

static DS_ERR_CODE DS_send_cmd_(DriverSuper* p_super, uint8_t stream);
static int      DS_tx_(DriverSuper* p_super, uint8_t stream);
static int      DS_rx_(DriverSuper* p_super);
static void     DS_analyze_rx_buffer_(DriverSuper* p_super,
                                      uint8_t stream,
                                      uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_prepare_buffer_(DriverSuper* p_super,
                                                     uint8_t stream,
                                                     uint8_t* rx_buffer,
                                                     uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_pickup_(DS_StreamConfig* p_stream_config,
                                             uint8_t* rx_buffer,
                                             uint16_t rec_data_len);
static void     DS_analyze_rx_buffer_carry_over_buffer_(DS_StreamConfig* p_stream_config,
                                                        uint8_t* rx_buffer,
                                                        uint16_t total_processed_data_len,
                                                        uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_fixed_pickup_(DS_StreamConfig* p_stream_config,
                                                   uint8_t* rx_buffer,
                                                   uint16_t total_processed_data_len,
                                                   uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_variable_pickup_with_rx_frame_size_(DS_StreamConfig* p_stream_config,
                                                                         uint8_t* rx_buffer,
                                                                         uint16_t total_processed_data_len,
                                                                         uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_variable_pickup_with_footer_(DS_StreamConfig* p_stream_config,
                                                                  uint8_t* rx_buffer,
                                                                  uint16_t total_processed_data_len,
                                                                  uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_finding_header_(DS_StreamConfig* p_stream_config,
                                                     uint8_t* rx_buffer,
                                                     uint16_t total_processed_data_len,
                                                     uint16_t rec_data_len);
static uint16_t DS_analyze_rx_buffer_receiving_header_(DS_StreamConfig* p_stream_config,
                                                       uint8_t* rx_buffer,
                                                       uint16_t total_processed_data_len);
static uint16_t DS_analyze_rx_buffer_receiving_footer_(DS_StreamConfig* p_stream_config,
                                                       uint8_t* rx_buffer,
                                                       uint16_t total_processed_data_len,
                                                       uint16_t rx_frame_size);
static uint32_t DS_analyze_rx_buffer_get_framelength_(DS_StreamConfig* p_stream_config);

static DS_ERR_CODE DS_reset_stream_config_(DS_StreamConfig* p_stream_config);
static DS_ERR_CODE DS_validate_stream_config_(DS_StreamConfig* p_stream_config);

// ダミー関数
// EQUだと関数ポインタの初期値をNULLにしていたためにぬるぽで事故ったので
static DS_ERR_CODE DS_load_init_setting_dummy_(DriverSuper* p_super);
static DS_ERR_CODE DS_data_analyzer_dummy_(DS_StreamConfig* p_stream_config, void* p_driver);


// ###### DriverSuper基本関数 ######

DS_ERR_CODE DS_init(DriverSuper* p_super, void* if_config, DS_ERR_CODE (*load_init_setting)(DriverSuper* p_super))
{
  if (DS_reset(p_super) != DS_ERR_CODE_OK) return DS_ERR_CODE_ERR;

  p_super->if_config = if_config;

  p_super->config.load_init_setting = load_init_setting;
  if (p_super->config.load_init_setting(p_super) != DS_ERR_CODE_OK) return DS_ERR_CODE_ERR;

  if (DS_validate_config(p_super) != DS_ERR_CODE_OK) return DS_ERR_CODE_ERR;

  // IFの初期化
  // 一旦シンプルにIF_initのエラーコードは無視する（実機でここでエラー出る場合はコードがおかしいので．必要があれば将来実装．）
  if ( (*IF_init[p_super->interface])(p_super->if_config) != 0 ) return DS_ERR_CODE_ERR;

  return DS_ERR_CODE_OK;
}


DS_ERR_CODE DS_reset(DriverSuper* p_super)
{
  uint8_t stream;

  p_super->interface = IF_LIST_MAX; // FIXME: (*IF_init[p_super->interface])(p_super->if_config) の様な使い方をするのでセグフォが起こる可能性があり
  p_super->if_config = NULL;        // FIXME: NULLポインタはこの関数がReset単体で使われるとマズい

  memset(p_super->config.rx_buffer_, 0x00, sizeof(p_super->config.rx_buffer_));

  p_super->config.load_init_setting = DS_load_init_setting_dummy_;

  p_super->config.rec_status_.ret_from_if_rx       = 0;
  p_super->config.rec_status_.rx_disruption_status = DS_RX_DISRUPTION_STATUS_OK;

  p_super->config.rx_count_      = 0;
  p_super->config.rx_call_count_ = 0;
  p_super->config.rx_time_       = TMGR_get_master_clock();

  p_super->config.should_monitor_for_rx_disruption_ = 0;
  p_super->config.time_threshold_for_rx_disruption_ = 60 * 1000;      // この値はよく考えること

  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    DS_ERR_CODE ret = DS_reset_stream_config_(&p_super->stream_config[stream]);
    if (ret != DS_ERR_CODE_OK) return ret;
  }
  return DS_ERR_CODE_OK;
}


DS_ERR_CODE DS_validate_config(DriverSuper* p_super)
{
  uint8_t stream;

  if (p_super->interface < 0 || p_super->interface >= IF_LIST_MAX) return DS_ERR_CODE_ERR;
  if (p_super->if_config == NULL) return DS_ERR_CODE_ERR;

  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    DS_ERR_CODE ret = DS_validate_stream_config_(&p_super->stream_config[stream]);
    if (ret != DS_ERR_CODE_OK) return ret;
  }

  return DS_ERR_CODE_OK;
}

DS_ERR_CODE DS_clear_rx_buffer(DriverSuper* p_super)
{
  uint8_t stream;

  // 以下，各種 buffer を memsetで念の為0クリアしておくが，
  // 情報は carry_over_buffer_size_ にあるので，動作上意味はない．
  memset(p_super->config.rx_buffer_, 0x00, sizeof(p_super->config.rx_buffer_));

  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    p_super->stream_config[stream].rx_frame_rec_len_ = 0;
    p_super->stream_config[stream].is_rx_buffer_carry_over_ = 0;
    p_super->stream_config[stream].carry_over_buffer_size_  = 0;

    memset(p_super->stream_config[stream].rx_frame_,
           0x00,
           sizeof(p_super->stream_config[stream].rx_frame_));
    memset(p_super->stream_config[stream].rx_buffer_for_carry_over_,
           0x00,
           sizeof(p_super->stream_config[stream].rx_buffer_for_carry_over_));
  }

  return DS_ERR_CODE_OK;
}


DS_ERR_CODE DS_receive(DriverSuper* p_super)
{
  uint8_t  stream;
  uint16_t rec_data_len;
  int      ret_rx;

  p_super->config.rx_call_count_++;

  // 各Driverで物理的に接続されている wire は１本なので，それをここで受信する．
  // 後段の stream では，その受信したビット列に対して，複数のフレーム種類に対して，フレーム探索，確定処理を走らす．
  ret_rx = DS_rx_(p_super);
  p_super->config.rec_status_.ret_from_if_rx = ret_rx;

  if (ret_rx > 0)
  {
    // なにかしらの受信データあり
    p_super->config.rx_count_++;
    p_super->config.rx_time_ = TMGR_get_master_clock();
  }

  // 受信途絶判定
  // テレメなどで見るときにノイズになるので，判定しないときは OK にしておく
  p_super->config.rec_status_.rx_disruption_status = DS_RX_DISRUPTION_STATUS_OK;
  if (p_super->config.should_monitor_for_rx_disruption_)
  {
    ObcTime now = TMGR_get_master_clock();
    uint32_t last_rx_ago = OBCT_diff_in_msec(&p_super->config.rx_time_, &now);

    if (last_rx_ago > p_super->config.time_threshold_for_rx_disruption_)
    {
      p_super->config.rec_status_.rx_disruption_status = DS_RX_DISRUPTION_STATUS_LOST;
    }
  }

  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);
    if (!p_stream_config->is_enabled_)
    {
      p_stream_config->rec_status_.status_code = DS_STREAM_REC_STATUS_DISABLE;
      continue;
    }

    // setter で validation かけると，初期化などで何度もかかることや，
    // そもそもこの validation は打ち上げ時というよりむしろ地上試験時に有用なので，ここに置く
    if (p_stream_config->is_validation_needed_for_rec_)
    {
      DS_ERR_CODE ret = DS_validate_stream_config_(p_stream_config);
      if (ret != DS_ERR_CODE_OK)
      {
        p_stream_config->rec_status_.status_code = DS_STREAM_REC_STATUS_VALIDATE_ERR;
        continue;
      }
    }

    if (ret_rx < 0)     // この共通処理が for の中にあるの，若干キモい．．．
    {
      // RX失敗
      p_stream_config->rec_status_.status_code = DS_STREAM_REC_STATUS_RX_ERR;
      // [TODO] ここに DriverSuper としての共通のアノマリ発行を入れるかは要議論
      //        個別対応なので， Driver に実装する，ないしは， IF 側に実装するのが自然か？
      continue;
    }
    else if (ret_rx == 0)
    {
      // 受信データなし
      // 繰越データがあれば処理する
      if (p_stream_config->is_rx_buffer_carry_over_)
      {
        // 繰越があるので，ここで continue せずへ次へ
      }
      else
      {
        // rec_status_.status_code 基本は更新せず
        // FIXED の場合はリセット
        if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
        {
          p_stream_config->rec_status_.status_code = DS_STREAM_REC_STATUS_FINDING_HEADER;
        }
        continue;
      }
    }
    else
    {
      // 何もしない
    }

    // ここまできたら受信データあり

    rec_data_len = (uint16_t)ret_rx;      // ここまでくれば非負数
    DS_analyze_rx_buffer_(p_super, stream, rec_data_len);

    // フレーム確定処理
    if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
    {
      p_stream_config->rx_frame_fix_count_++;
      p_stream_config->rx_frame_fix_time_ = TMGR_get_master_clock();
      p_stream_config->req_tlm_cmd_tx_count_after_last_tx_ = 0;
    }
  }

  // streamのテレメ途絶判定（テレメフレーム確定途絶判定）
  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);
    ObcTime now;
    uint32_t last_tlm_fix_ago;

    // テレメなどで見るときにノイズになるので，判定しないときは OK にしておく
    p_stream_config->rec_status_.tlm_disruption_status = DS_STREAM_TLM_DISRUPTION_STATUS_OK;

    if (!p_stream_config->is_enabled_ || !p_stream_config->should_monitor_for_tlm_disruption_)
    {
      continue;
    }

    now = TMGR_get_master_clock();
    last_tlm_fix_ago = OBCT_diff_in_msec(&p_stream_config->rx_frame_fix_time_, &now);

    if (last_tlm_fix_ago > p_stream_config->time_threshold_for_tlm_disruption_)
    {
      p_stream_config->rec_status_.tlm_disruption_status = DS_STREAM_TLM_DISRUPTION_STATUS_LOST;
    }
  }

  return (ret_rx < 0) ? DS_ERR_CODE_ERR : DS_ERR_CODE_OK;
}


DS_ERR_CODE DS_analyze_rec_data(DriverSuper* p_super, uint8_t stream, void* p_driver)
{
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);

  p_stream_config->ret_from_data_analyzer_ = p_stream_config->data_analyzer_(p_stream_config, p_driver);

  return p_stream_config->ret_from_data_analyzer_;
}


DS_ERR_CODE DS_send_general_cmd(DriverSuper* p_super, uint8_t stream)
{
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);

  if (!p_stream_config->is_enabled_)
  {
    p_stream_config->send_status_.status_code = DS_STREAM_SEND_STATUS_DISABLE;
    return DS_ERR_CODE_OK;
  }

  p_stream_config->general_cmd_tx_count_++;
  p_stream_config->general_cmd_tx_time_ = TMGR_get_master_clock();

#ifdef DS_DEBUG
  Printf("DS: send_general_cmd\n");
#endif

  return DS_send_cmd_(p_super, stream);
}


DS_ERR_CODE DS_send_req_tlm_cmd(DriverSuper* p_super, uint8_t stream)
{
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);

  if (!p_stream_config->is_enabled_)
  {
    p_stream_config->send_status_.status_code = DS_STREAM_SEND_STATUS_DISABLE;
    return DS_ERR_CODE_OK;
  }

  p_stream_config->req_tlm_cmd_tx_count_++;
  p_stream_config->req_tlm_cmd_tx_count_after_last_tx_++;
  p_stream_config->req_tlm_cmd_tx_time_ = TMGR_get_master_clock();

#ifdef DS_DEBUG
  Printf("DS: send_req_tlm_cmd\n");
#endif

  return DS_send_cmd_(p_super, stream);
}


/**
 * @brief  コマンド送信処理
 *
 *         DS_send_general_cmd と DS_send_req_tlm_cmdの共通部分
 * @param  *p_super DriverSuper構造体へのポインタ
 * @param  stream   どのconfigを使用するか．streamは0-MAXなので，継承先でENUMなど宣言して使いやすくすればいいと思う．
 * @retval DS_ERR_CODE_OK  : 正常終了
 * @retval DS_ERR_CODE_ERR : IF_TX でのエラーあり
 * @note   受信状況やエラー情報は send_status_ に格納されている
 */
static DS_ERR_CODE DS_send_cmd_(DriverSuper* p_super, uint8_t stream)
{
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);

  // setter で validation かけると，初期化などで何度もかかることや，
  // そもそもこの validation は打ち上げじというよりむしろ地上試験時に有用なので，ここに置く
  if (p_stream_config->is_validation_needed_for_send_)
  {
    DS_ERR_CODE ret = DS_validate_stream_config_(p_stream_config);
    if (ret != DS_ERR_CODE_OK)
    {
      p_stream_config->send_status_.status_code = DS_STREAM_SEND_STATUS_VALIDATE_ERR;
      return DS_ERR_CODE_ERR;
    }
  }

  p_stream_config->send_status_.ret_from_if_tx = DS_tx_(p_super, stream);

  if (p_stream_config->send_status_.ret_from_if_tx != 0)
  {
    p_stream_config->send_status_.status_code = DS_STREAM_SEND_STATUS_TX_ERR;
    return DS_ERR_CODE_ERR;
  }

  p_stream_config->send_status_.status_code = DS_STREAM_SEND_STATUS_OK;
  return DS_ERR_CODE_OK;
}

/**
 * @brief  継承先の機器にコマンドを発行する
 * @note   この関数の実行前に，tx_frame_, tx_frame_size_の設定が必要である
 * @param  *p_super DriverSuper構造体へのポインタ
 * @param  stream   どのconfigを使用するか．streamは0-MAXなので，継承先でENUMなど宣言して使いやすくすればいいと思う．
 * @retval DS_ERR_CODE_OK (0) : 正常終了
 * @retval 0以外 : IF_TX の戻り値
 */
static int DS_tx_(DriverSuper* p_super, uint8_t stream)
{
  int ret;
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);

  if (p_stream_config->tx_frame_size_ == 0) return DS_ERR_CODE_OK;
  if (p_stream_config->tx_frame_ == NULL) return DS_ERR_CODE_OK;

#ifdef DS_DEBUG
  Printf("DS: tx_\n");
#endif

  ret = (*IF_TX[p_super->interface])(p_super->if_config,
                                     p_stream_config->tx_frame_,
                                     (int)p_stream_config->tx_frame_size_);

  if (ret != 0) return ret;
  return DS_ERR_CODE_OK;
}


/**
 * @brief  継承先の機器からの受信データがあるか確認し，受信する
 * @param  *p_super DriverSuper構造体へのポインタ
 * @retval 0    : 受信データなし
 * @retval 正数 : 受信データ長 [Byte]
 * @retval 負数 : IF_RXのエラー
 */
static int DS_rx_(DriverSuper* p_super)
{
  int flag;
  int rec_data_len;
  uint8_t stream;
#ifdef DS_DEBUG_SHOW_REC_DATA
  int16_t i;
#endif

  // 少なくとも１つのstreamが有効でかつ，rx_frame_size_が0以外でないと受信処理はしない
  flag = 0;
  for (stream = 0; stream < DS_STREAM_MAX; ++stream)
  {
    if (! (p_super->stream_config[stream].is_enabled_) ) continue;
    if (p_super->stream_config[stream].rx_frame_size_ != 0)
    {
      flag = 1;
    }
  }
  if (flag == 0) return 0;

  rec_data_len = (*IF_RX[p_super->interface])(p_super->if_config,
                                              p_super->config.rx_buffer_,
                                              DS_RX_BUFFER_SIZE_MAX);

#ifdef DS_DEBUG
  Printf("DS: rx_\n");
#endif

  if (rec_data_len <= 0) return rec_data_len;     // 負数はエラーコード

#ifdef DS_DEBUG_SHOW_REC_DATA
  Printf("DS: Receive data size is %d bytes, as follow:\n", rec_data_len);
  for (i = 0; i < rec_data_len; i++)
  {
    Printf("%02x ", p_super->config.rx_buffer_[i]);
    if (i % 4 == 3) Printf("   ");
  }
  Printf("\n");
#endif

  return rec_data_len;
}


/**
 * @brief  受信フレーム解析関数
 * @param  *p_super      DriverSuper構造体へのポインタ
 * @param  stream        どのconfigを使用するか．streamは0-MAXなので，継承先でENUMなど宣言して使いやすくすればいいと思う．
 * @param  rec_data_len  受信データのバッファの長さ
 * @return void 詳細は DS_StreamRecStatus
 */
static void DS_analyze_rx_buffer_(DriverSuper* p_super,
                                  uint8_t stream,
                                  uint16_t rec_data_len)
{
  // 解析用受信バッファ
  // 巨大なデータなので，staticで予め確保しておき，実行時のスタック枯渇を避ける
  static uint8_t rx_buffer[DS_RX_BUFFER_SIZE_MAX * 2];
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);
  uint16_t total_processed_data_len;

  rec_data_len = DS_analyze_rx_buffer_prepare_buffer_(p_super, stream, rx_buffer, rec_data_len);

  total_processed_data_len = DS_analyze_rx_buffer_pickup_(p_stream_config, rx_buffer, rec_data_len);

  DS_analyze_rx_buffer_carry_over_buffer_(p_stream_config, rx_buffer, total_processed_data_len, rec_data_len);
}


/**
 * @brief  解析用受信バッファの準備
 *
 *         繰り越されたデータと今回受信したデータの結合を行い，受信データ解析の準備をする
 * @param[in]  *p_super      DriverSuper構造体へのポインタ
 * @param[in]  stream        どのconfigを使用するか．streamは0-MAXなので，継承先でENUMなど宣言して使いやすくすればいいと思う．
 * @param[out] *rx_buffer    解析用受信バッファ
 * @param[in]  rec_data_len  受信データのバッファの長さ
 * @return 解析用受信バッファの長さ
 */
static uint16_t DS_analyze_rx_buffer_prepare_buffer_(DriverSuper* p_super,
                                                     uint8_t stream,
                                                     uint8_t* rx_buffer,
                                                     uint16_t rec_data_len)
{
  DS_StreamConfig* p_stream_config = &(p_super->stream_config[stream]);
  uint16_t buffer_offset = 0;

  // 繰越受信データのとりこみ
  if (p_stream_config->is_rx_buffer_carry_over_)
  {
    memcpy(rx_buffer,
           p_stream_config->rx_buffer_for_carry_over_,
           (size_t)p_stream_config->carry_over_buffer_size_);
    buffer_offset += p_stream_config->carry_over_buffer_size_;
  }

  // 今回受信分のとりこみ
  memcpy(&(rx_buffer[buffer_offset]),
         p_super->config.rx_buffer_,
         (size_t)rec_data_len);

  if (p_stream_config->is_rx_buffer_carry_over_)
  {
    rec_data_len += p_stream_config->carry_over_buffer_size_;
  }

  return rec_data_len;
}


/**
 * @brief  フレーム解析関数
 *
 *         解析用受信バッファを走査し，バイト単位でフレーム内データを拾っていく
 * @param  p_stream_config DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer       受信データのバッファ（配列）
 * @param  rec_data_len    受信データのバッファの長さ
 * @return 今回の呼び出しで走査したバイト長さ．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_pickup_(DS_StreamConfig* p_stream_config,
                                             uint8_t* rx_buffer,
                                             uint16_t rec_data_len)
{
  uint16_t total_processed_data_len;
  // 受信バッファのデータを走査し，必要なデータをフレームとしてpickupする関数
  uint16_t (*pickup_func)(DS_StreamConfig* p_stream_config,
                          uint8_t* rx_buffer,
                          uint16_t total_processed_data_len,
                          uint16_t rec_data_len);
  p_stream_config->rx_frame_head_pos_of_frame_candidate_ = 0;

  // TODO: ビッグデータ対応
  if (p_stream_config->rx_frame_size_ > 0 && p_stream_config->rx_frame_size_ < DS_RX_FRAME_SIZE_MAX)
  {
    pickup_func = DS_analyze_rx_buffer_fixed_pickup_;
  }
  else if (p_stream_config->rx_frame_size_ > 0)
  {
    // DS_analyze_rx_buffer_fixed_bigdata_(void)
    pickup_func = NULL;
    return rec_data_len;
  }
  else if (p_stream_config->rx_frame_size_ < 0 && p_stream_config->rx_frame_size_ < DS_RX_FRAME_SIZE_MAX)
  {
    // フレームにフレーム長データが含まれているか？
    if (p_stream_config->rx_framelength_pos_ >= 0)
    {
      pickup_func = DS_analyze_rx_buffer_variable_pickup_with_rx_frame_size_;
    }
    else
    {
      pickup_func = DS_analyze_rx_buffer_variable_pickup_with_footer_;
    }
  }
  else
  {
    // DS_analyze_rx_buffer_variable_bigdata_(void)
    pickup_func = NULL;
    return rec_data_len;
  }

  // 受信バッファからデータをピックアップしていく
  total_processed_data_len = p_stream_config->carry_over_buffer_next_pos_;
  while (total_processed_data_len < rec_data_len)
  {
    uint16_t processed_data_len = pickup_func(p_stream_config,
                                              rx_buffer,
                                              total_processed_data_len,
                                              rec_data_len);
    total_processed_data_len += processed_data_len;

    if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
    {
      break;
    }

    // 不整合が起きたら，現在のframe候補の先頭 + 1バイト目に走査場所を戻す
    if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_HEADER_MISMATCH ||
        p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FOOTER_MISMATCH ||
        p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_RX_FRAME_TOO_LONG ||
        p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_RX_FRAME_TOO_SHORT)
    {
      total_processed_data_len = (uint16_t)(p_stream_config->rx_frame_head_pos_of_frame_candidate_ + 1);

      // 他の部分での条件分岐のために，詳細エラー情報を現在のステータスに上書きする
      p_stream_config->rec_status_.status_code = DS_STREAM_REC_STATUS_FINDING_HEADER;
    }

    // processed_data_len > rec_data_lenなることはありえないが，念の為チャックする？？
  }

  return total_processed_data_len;
}


/**
 * @brief  フレーム解析関数後のデータ繰越関数
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rec_data_len             受信データのバッファの長さ
 * @return void
 */
static void DS_analyze_rx_buffer_carry_over_buffer_(DS_StreamConfig* p_stream_config,
                                                    uint8_t* rx_buffer,
                                                    uint16_t total_processed_data_len,
                                                    uint16_t rec_data_len)
{
  // 受信データを次回呼び出しに引き継ぐ（いくつかのパターンあり）
  // フレーム確定した場合（未解析受信データがある可能性がある）
  //     厳格なフレーム探索が無効
  //          --> フレーム確定したデータはOKとし，それ以後の解析できなかった受信データのみを次回に引き継ぐ
  //     厳格なフレーム探索が有効
  //          --> 確定フレームがユーザー側で弾かれる可能性を考慮し，確定フレームの先頭 + 1 バイト目以降を次回に引き継ぐ
  //              次回のヘッダ探索はそこから始まる
  // フレーム確定しなかった場合（未解析受信データはなし）
  //     DS_STREAM_REC_STATUS_FINDING_HEADER のとき
  //          --> ヘッダがなかったということなので，引き継ぎデータはなし
  //     DS_STREAM_REC_STATUS_FINDING_HEADER でないとき
  //          --> フレーム候補のフレーム先頭以降を次回に引き継ぐ
  //              これにより，フッタ不一致などの不整合が発生した場合にフレーム探索をやり直せる

  p_stream_config->carry_over_buffer_size_ = 0;
  if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FIXED_FRAME)
  {
    if (p_stream_config->is_strict_frame_search_)
    {
      // 確定フレームの先頭 + 1 バイト目以降を次回に引き継ぐ
      p_stream_config->carry_over_buffer_size_ = (uint16_t)(rec_data_len - p_stream_config->rx_frame_head_pos_of_frame_candidate_ - 1);
      // 次回は，引き継いだデータの先頭から再びフレーム探索
      p_stream_config->carry_over_buffer_next_pos_ = 0;
    }
    else
    {
      // フレーム確定して，解析できなかった受信データがある場合，次回に引き継ぐ
      p_stream_config->carry_over_buffer_size_ = (uint16_t)(rec_data_len - total_processed_data_len);
      // 次回は，引き継いだデータの先頭から再びフレーム探索
      p_stream_config->carry_over_buffer_next_pos_ = 0;
    }
  }
  else
  {
    if (p_stream_config->rec_status_.status_code == DS_STREAM_REC_STATUS_FINDING_HEADER)
    {
      // 引き継ぎデータはなし
      p_stream_config->carry_over_buffer_size_ = 0;
      // 次回は，先頭から再びフレーム探索
      p_stream_config->carry_over_buffer_next_pos_ = 0;
    }
    else
    {
      // 確定フレームの先頭以降を次回に引き継ぐ
      p_stream_config->carry_over_buffer_size_ = (uint16_t)(rec_data_len - p_stream_config->rx_frame_head_pos_of_frame_candidate_);
      // 次回は，引き継いだデータはスキップし，受信したものの先頭からフレーム
      p_stream_config->carry_over_buffer_next_pos_ = p_stream_config->carry_over_buffer_size_;
    }
  }

  if (p_stream_config->carry_over_buffer_size_ > 0 && p_stream_config->carry_over_buffer_size_ <= DS_RX_BUFFER_SIZE_MAX)
  {
    uint16_t pos = (uint16_t)(rec_data_len - p_stream_config->carry_over_buffer_size_);
    p_stream_config->is_rx_buffer_carry_over_ = 1;
    memcpy(p_stream_config->rx_buffer_for_carry_over_,
           &(rx_buffer[pos]),
           (size_t)p_stream_config->carry_over_buffer_size_);
  }
  else
  {
    // 引き継ぐサイズが DS_RX_BUFFER_SIZE_MAX を超えた場合，処理のキャパを超えてしまっているので，リセット．
    if (p_stream_config->carry_over_buffer_size_ > 0)
    {
      p_stream_config->rec_status_.count_of_carry_over_failures++;
    }
    p_stream_config->is_rx_buffer_carry_over_    = 0;
    p_stream_config->carry_over_buffer_size_     = 0;
    p_stream_config->carry_over_buffer_next_pos_ = 0;
  }

  return;
}


/**
 * @brief  固定長フレーム解析関数（バイト列処理）
 *
 *         受信バッファのデータを走査し，必要なデータをフレームとしてpickupする
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rec_data_len             受信データのバッファの長さ
 * @return 今回の呼び出しで走査したバイト長さ．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_fixed_pickup_(DS_StreamConfig* p_stream_config,
                                                   uint8_t* rx_buffer,
                                                   uint16_t total_processed_data_len,
                                                   uint16_t rec_data_len)
{
  uint16_t unprocessed_data_len = (uint16_t)(rec_data_len - total_processed_data_len);      // このキャストは若干危ない（コードが論理的に正しければ問題ないが）
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...

  if (p->rx_frame_rec_len_ == 0 && p->rx_header_size_ != 0)
  {
    // まだヘッダの先頭すら未発見の場合（ヘッダなし時はここはスキップ）
    return DS_analyze_rx_buffer_finding_header_(p_stream_config,
                                                rx_buffer,
                                                total_processed_data_len,
                                                rec_data_len);
  }
  else if (p->rx_frame_rec_len_ < p->rx_header_size_)
  {
    // ヘッダ受信中
    return DS_analyze_rx_buffer_receiving_header_(p_stream_config,
                                                  rx_buffer,
                                                  total_processed_data_len);
  }
  else if (p->rx_frame_rec_len_ < p->rx_frame_size_ - p->rx_footer_size_)
  {
    // データ受信中
    // ここは高速化のために一括処理
    uint16_t pickup_data_len;

    // ヘッダなしの場合は，ここがフレーム先頭
    if (p->rx_frame_rec_len_ == 0)
    {
      p->rx_frame_head_pos_of_frame_candidate_ = total_processed_data_len;
    }

    pickup_data_len = (uint16_t)(p->rx_frame_size_ - p->rx_footer_size_ - p->rx_frame_rec_len_);

    // 今回で全部受信しきらない場合
    if (pickup_data_len > unprocessed_data_len)
    {
      pickup_data_len = unprocessed_data_len;
    }

    memcpy(&(p->rx_frame_[p->rx_frame_rec_len_]),
           &(rx_buffer[total_processed_data_len]),
           (size_t)pickup_data_len);

    p->rx_frame_rec_len_ += pickup_data_len;
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_DATA;

    // フッタがなく，data受信仕切った場合はフレーム確定
    // これがないと，DS_analyze_rx_buffer_fixed_ で
    // 今まさに受信したデータ長がぴったりフレーム末だった場合に，フレーム確定が１周期遅れることになるので
    if (p->rx_footer_size_ == 0 && p->rx_frame_rec_len_ == p->rx_frame_size_)
    {
      p->rec_status_.status_code = DS_STREAM_REC_STATUS_FIXED_FRAME;
      p->rec_status_.fixed_frame_len = p->rx_frame_rec_len_;
      p->rx_frame_rec_len_ = 0;
    }

    return pickup_data_len;
  }
  else
  {
    // フッタ受信中 or フッタなしの場合はフレーム確定
    return DS_analyze_rx_buffer_receiving_footer_(p_stream_config,
                                                  rx_buffer,
                                                  total_processed_data_len,
                                                  (uint16_t)(p->rx_frame_size_));
  }
}


/**
 * @brief  可変フレーム解析関数（バイト列処理）
 *
 *         受信バッファのデータを走査し，必要なデータをフレームとしてpickupする
 * @note   受信フレームにフレーム長データが存在していることを前提とする
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rec_data_len             受信データのバッファの長さ
 * @return 今回の呼び出しで走査したバイト長さ．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_variable_pickup_with_rx_frame_size_(DS_StreamConfig* p_stream_config,
                                                                         uint8_t* rx_buffer,
                                                                         uint16_t total_processed_data_len,
                                                                         uint16_t rec_data_len)
{
  uint16_t unprocessed_data_len = (uint16_t)(rec_data_len - total_processed_data_len);      // このキャストは若干危ない（コードが論理的に正しければ問題ないが）
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...
  uint32_t rx_frame_size = DS_analyze_rx_buffer_get_framelength_(p_stream_config);      // まだ受信していない場合は不定値が入ることに留意すること！！

  if (p->rx_frame_rec_len_ == 0 && p->rx_header_size_ != 0)
  {
    // まだヘッダの先頭すら未発見の場合（ヘッダなし時はここはスキップ）
    return DS_analyze_rx_buffer_finding_header_(p_stream_config,
                                                rx_buffer,
                                                total_processed_data_len,
                                                rec_data_len);
  }
  else if (p->rx_frame_rec_len_ < p->rx_header_size_)
  {
    // ヘッダ受信中
    return DS_analyze_rx_buffer_receiving_header_(p_stream_config,
                                                  rx_buffer,
                                                  total_processed_data_len);
  }
  else if (p->rx_frame_rec_len_ < p->rx_framelength_pos_ + p->rx_framelength_type_size_)
  {
    // フレームサイズ探索中
    // ここは高速化のために一括処理
    uint16_t pickup_data_len;

    // ヘッダなしの場合は，ここがフレーム先頭
    if (p->rx_frame_rec_len_ == 0)
    {
      p->rx_frame_head_pos_of_frame_candidate_ = total_processed_data_len;
    }

    pickup_data_len = (uint16_t)(p->rx_framelength_pos_ + p->rx_framelength_type_size_ - p->rx_frame_rec_len_);

    // 今回で全部受信しきらない場合
    if (pickup_data_len > unprocessed_data_len)
    {
      pickup_data_len = unprocessed_data_len;
    }

    memcpy(&(p->rx_frame_[p->rx_frame_rec_len_]),
           &(rx_buffer[total_processed_data_len]),
           (size_t)pickup_data_len);

    p->rx_frame_rec_len_ += pickup_data_len;
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_FRAMELENGTH;

    // フレーム長を受信し終わった場合，チェックする
    if (p->rx_frame_rec_len_ >= p->rx_framelength_pos_ + p->rx_framelength_type_size_)
    {
      rx_frame_size = DS_analyze_rx_buffer_get_framelength_(p_stream_config);

      // バッファー超えはエラーを出す！
      if (rx_frame_size > DS_RX_FRAME_SIZE_MAX)
      {
        p->rec_status_.status_code = DS_STREAM_REC_STATUS_RX_FRAME_TOO_LONG;
        p->rx_frame_rec_len_ = 0;
#ifdef DS_DEBUG
        Printf("DS: RX frame size is too long\n");
#endif
        return 0;       // TODO: ここで何を返すかはもうすこし考えても良いかも？
      }

      // bodyサイズがは0以上を要請
      if (rx_frame_size < p->rx_header_size_ + p->rx_footer_size_)
      {
        p->rec_status_.status_code = DS_STREAM_REC_STATUS_RX_FRAME_TOO_SHORT;
        p->rx_frame_rec_len_ = 0;
#ifdef DS_DEBUG
        Printf("DS: RX frame size is too short\n");
#endif
        return 0;       // TODO: ここで何を返すかはもうすこし考えても良いかも？
      }
    }

    return pickup_data_len;
  }
  else if (p->rx_frame_rec_len_ < rx_frame_size - p->rx_footer_size_)
  {
    // データ受信中
    // ここは高速化のために一括処理

    uint16_t pickup_data_len = (uint16_t)(rx_frame_size - p->rx_footer_size_ - p->rx_frame_rec_len_);    // TODO: 現在，フレーム長がuint16_tを超えることは想定していない！

    // 今回で全部受信しきらない場合
    if (pickup_data_len > unprocessed_data_len)
    {
      pickup_data_len = unprocessed_data_len;
    }

    memcpy(&(p->rx_frame_[p->rx_frame_rec_len_]),
           &(rx_buffer[total_processed_data_len]),
           (size_t)pickup_data_len);

    p->rx_frame_rec_len_ += pickup_data_len;
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_DATA;

    // フッタがなく，data受信仕切った場合はフレーム確定
    // これがないと，DS_analyze_rx_buffer_fixed_ で今まさに受信したデータ長がぴったりフレーム末だった場合に，フレーム確定が１周期遅れることになるので
    if (p->rx_footer_size_ == 0 && p->rx_frame_rec_len_ == rx_frame_size)
    {
      p->rec_status_.status_code = DS_STREAM_REC_STATUS_FIXED_FRAME;
      p->rx_frame_rec_len_ = 0;
    }

    return pickup_data_len;
  }
  else
  {
    // フッタ受信中 or フッタなしの場合はフレーム確定
    return DS_analyze_rx_buffer_receiving_footer_(p_stream_config,
                                                  rx_buffer,
                                                  total_processed_data_len,
                                                  (uint16_t)rx_frame_size);
  }
}


/**
 * @brief  可変フレーム解析関数（バイト列処理）
 *
 *         受信バッファのデータを走査し，必要なデータをフレームとしてpickupする
 * @note   DS_analyze_rx_buffer_variable_pickup_with_rx_frame_size_ との違いは，テレメ長データがフレームに含まれるか否か
 * @note   フッタが存在していることを前提とする
 * @note   ヘッダなしは認める．ただし，受信データ先頭からフレームとみなすので，ヘッダありを強く推奨する
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rec_data_len             受信データのバッファの長さ
 * @return 今回の呼び出しで走査したバイト長さ．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_variable_pickup_with_footer_(DS_StreamConfig* p_stream_config,
                                                                  uint8_t* rx_buffer,
                                                                  uint16_t total_processed_data_len,
                                                                  uint16_t rec_data_len)
{
  uint16_t unprocessed_data_len = (uint16_t)(rec_data_len - total_processed_data_len);      // このキャストは若干危ない（コードが論理的に正しければ問題ないが）
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...

  if (p->rx_frame_rec_len_ == 0 && p->rx_header_size_ != 0)
  {
    // まだヘッダの先頭すら未発見の場合（ヘッダなし時はここはスキップ）
    return DS_analyze_rx_buffer_finding_header_(p_stream_config,
                                                rx_buffer,
                                                total_processed_data_len,
                                                rec_data_len);
  }
  else if (p->rx_frame_rec_len_ < p->rx_header_size_)
  {
    // ヘッダ受信中
    return DS_analyze_rx_buffer_receiving_header_(p_stream_config,
                                                  rx_buffer,
                                                  total_processed_data_len);
  }
  else
  {
    // 最後まで受信し，フッタの最終文字を探す．フッタなしはありえない．
    uint8_t* p_footer_last;     // inclusive
    int32_t  body_data_len;     // サイズ的にはu16でよいが，負数もとりたいのでi32としている
    uint16_t processed_data_len;
    uint16_t i;
    uint16_t estimated_rx_frame_size;
    uint16_t pickup_data_len;

    // ヘッダなしの場合は，ここがフレーム先頭
    if (p->rx_frame_rec_len_ == 0)
    {
      p->rx_frame_head_pos_of_frame_candidate_ = total_processed_data_len;
    }

    // 届いているデータを受信フレームバッファに格納する
    // ここは高速化のために一括処理
    pickup_data_len = unprocessed_data_len;
    // 永遠にフッタを受信しない場合にバッファーオーバーランすることを防ぐ
    if (p->rx_frame_rec_len_ + pickup_data_len > DS_RX_FRAME_SIZE_MAX)
    {
      if (p->rx_frame_rec_len_ >= DS_RX_FRAME_SIZE_MAX)
      {
        // これ以上受信できないため，フッタ探索失敗として，リセットする
        p->rec_status_.status_code = DS_STREAM_REC_STATUS_RX_FRAME_TOO_LONG;
        p->rx_frame_rec_len_ = 0;
#ifdef DS_DEBUG
        Printf("DS: RX frame is too long\n");
#endif
        return 0;   // 処理済みデータもなし
      }
      pickup_data_len = (uint16_t)(DS_RX_FRAME_SIZE_MAX - p->rx_frame_rec_len_);
    }
    memcpy(&(p->rx_frame_[p->rx_frame_rec_len_]),
           &(rx_buffer[total_processed_data_len]),
           (size_t)pickup_data_len);

    // フッタ最終文字を探す
    p_footer_last = (uint8_t*)memchr(&(rx_buffer[p->rx_frame_rec_len_]),
                                     (int)(p->rx_footer_[p->rx_footer_size_ - 1]),
                                     (size_t)pickup_data_len);

    if (p_footer_last == NULL)
    {
      // まだまだ受信する
      p->rx_frame_rec_len_ += pickup_data_len;
      p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_DATA;
      return pickup_data_len;
    }

    processed_data_len = (uint16_t)(p_footer_last - &(rx_buffer[p->rx_frame_rec_len_]) + 1);
    body_data_len = (p_footer_last - rx_buffer + 1) - p->rx_header_size_ - p->rx_footer_size_;
    if (body_data_len < 0)
    {
      // これはフッタではないので受信続行
      // まだまだ受信する
      p->rx_frame_rec_len_ += pickup_data_len;
      p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_DATA;
      return pickup_data_len;
    }

    // フッタ候補発見
    // フッタチェックする
    estimated_rx_frame_size = (uint16_t)(p->rx_header_size_ + body_data_len + p->rx_footer_size_);
    for (i = 0; i < p->rx_footer_size_; i++)
    {
      if (rx_buffer[estimated_rx_frame_size - i - 1] != p->rx_footer_[p->rx_footer_size_ - i - 1])
      {
        // これはフッタではないので受信続行
        // まだまだ受信する
        p->rx_frame_rec_len_ += pickup_data_len;
        p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_DATA;
        return pickup_data_len;
      }
    }

    // フッタ確定 → フレーム確定
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_FIXED_FRAME;
    p->rec_status_.fixed_frame_len = estimated_rx_frame_size;
    p->rx_frame_rec_len_ = 0;
    return processed_data_len;
  }
}


/**
 * @brief  フレーム解析関数（ヘッダ探索）
 * @note   ヘッダが見つかった場合，最初の1 byteのみ処理する
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rec_data_len             受信データのバッファの長さ
 * @return 今回の呼び出しで走査したバイト長さ．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_finding_header_(DS_StreamConfig* p_stream_config,
                                                     uint8_t* rx_buffer,
                                                     uint16_t total_processed_data_len,
                                                     uint16_t rec_data_len)
{
  uint16_t unprocessed_data_len = (uint16_t)(rec_data_len - total_processed_data_len);      // このキャストは若干危ない（コードが論理的に正しければ問題ないが）
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...
  uint8_t* p_header;
  uint16_t processed_data_len;

  // コードが正しければ，ヘッダがないケースはここに到達し得ないが，ヌルポ回避をいれておく
  if (p_stream_config->rx_header_ == NULL)
  {
#ifdef DS_DEBUG
    Printf("DS: RX header is invalid\n");
#endif
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_OTHER_ERR;
    return unprocessed_data_len;
  }

  // まだヘッダの先頭すら未発見の場合
  // ここは高速化のために一括処理
  p_header = (uint8_t*)memchr(&(rx_buffer[total_processed_data_len]),
                              (int)(p->rx_header_[0]),
                              (size_t)unprocessed_data_len);

  if (p_header == NULL)
  {
#ifdef DS_DEBUG
    Printf("DS: RX header not found\n");
#endif
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_FINDING_HEADER;
    return unprocessed_data_len;
  }

  processed_data_len = (uint16_t)(p_header - &(rx_buffer[total_processed_data_len]) + 1);

  // ヘッダコピー．ホントはbufferからコピるべきだけど，ちょっとアドレスいじっていて怖いので．．．
  p->rx_frame_[p->rx_frame_rec_len_] = p->rx_header_[0];
  p->rx_frame_rec_len_++;

  p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_HEADER;
  p->rx_frame_head_pos_of_frame_candidate_ = (uint16_t)(total_processed_data_len + processed_data_len - 1);
  return processed_data_len;
}


/**
 * @brief  フレーム解析関数（ヘッダ受信中）
 * @note   1 byteのみ処理する
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @return 今回の呼び出しで走査したバイト長さ（まあ，1なんだけど）．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_receiving_header_(DS_StreamConfig* p_stream_config,
                                                       uint8_t* rx_buffer,
                                                       uint16_t total_processed_data_len)
{
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...

  // ヘッダ受信中
  // 受信が細切れのときなどの処理分岐がめんどくさいので，1byteずつ処理させる

  // ヘッダが正しいか？
  if (rx_buffer[total_processed_data_len] == p->rx_header_[p->rx_frame_rec_len_])
  {
    p->rx_frame_[p->rx_frame_rec_len_] = p->rx_header_[p->rx_frame_rec_len_];
    p->rx_frame_rec_len_++;

    p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_HEADER;
    return 1;
  }
  else
  {
    // ヘッダが正しくなかった
    // DS_STREAM_REC_STATUS_HEADER_MISMATCH になり，再びバッファを巻き戻してヘッダ探索を始める
    // その後 DS_STREAM_REC_STATUS_FINDING_HEADER に戻る
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_HEADER_MISMATCH;
    p->rx_frame_rec_len_ = 0;
#ifdef DS_DEBUG
    Printf("DS: RX header is mismatch\n");
#endif
    return 1;
  }
}


/**
 * @brief  フレーム解析関数（フッタ受信中）
 * @note   1 byteのみ処理する
 * @note   現在，フレーム長がuint16_tを超えることは想定していない！
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @param  rx_buffer                受信データのバッファ（配列）
 * @param  total_processed_data_len 受信データのバッファのうち，すでに処理されたバイト数
 * @param  rx_frame_size            フレームサイズ（可変長フレームの場合もあるので，引数に取る）
 * @return 今回の呼び出しで走査したバイト長さ（まあ，1なんだけど）．その他の詳細は DS_StreamRecStatus
 */
static uint16_t DS_analyze_rx_buffer_receiving_footer_(DS_StreamConfig* p_stream_config,
                                                       uint8_t* rx_buffer,
                                                       uint16_t total_processed_data_len,
                                                       uint16_t rx_frame_size)
{
  DS_StreamConfig* p = p_stream_config;  // ちょっと変数名が長すぎて配列indexなどがみずらいので...
  uint16_t rec_footer_pos;

  // フッタなしの場合はフレーム確定
  if (p->rx_footer_size_ == 0)
  {
    // フッタなし
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_FIXED_FRAME;
    p->rec_status_.fixed_frame_len = p->rx_frame_rec_len_;
    p->rx_frame_rec_len_ = 0;
    return 0;   // 処理済みデータもなし
  }

  // フッタ受信
  // ここも条件分岐がめんどくさいので，1byteずつ処理する
  rec_footer_pos = (uint16_t)(p->rx_frame_rec_len_ - (rx_frame_size - p->rx_footer_size_));

  // 期待されているフッタが受信できたか？
  // 受信できなかった場合， DS_STREAM_REC_STATUS_FOOTER_MISMATCH になり，再びバッファを巻き戻してヘッダ探索を始める
  // その後 DS_STREAM_REC_STATUS_FINDING_HEADER に戻る
  if (rx_buffer[total_processed_data_len] != p->rx_footer_[rec_footer_pos])
  {
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_FOOTER_MISMATCH;
    p->rx_frame_rec_len_ = 0;
#ifdef DS_DEBUG
    Printf("DS: RX footer is mismatch\n");
#endif
    return 1;
  }

  // ここまできたら正しいフッタが受信されている
  p->rx_frame_[p->rx_frame_rec_len_] = p->rx_footer_[rec_footer_pos];
  p->rx_frame_rec_len_++;

  if (p->rx_frame_rec_len_ == rx_frame_size)
  {
    // フレーム確定
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_FIXED_FRAME;
    p->rec_status_.fixed_frame_len = p->rx_frame_rec_len_;
    p->rx_frame_rec_len_ = 0;
  }
  else
  {
    // フレーム確定せず
    p->rec_status_.status_code = DS_STREAM_REC_STATUS_RECEIVING_FOOTER;
  }

  return 1;
}


/**
 * @brief  フレーム解析中に受信したフレームからフレーム長を取得する関数
 * @note   DS_analyze_rx_buffer_variable_pickup_with_rx_frame_size_ から呼ばれることを想定
 * @param  p_stream_config          DriverSuper構造体のDS_StreamConfig
 * @return フレーム長
 */
static uint32_t DS_analyze_rx_buffer_get_framelength_(DS_StreamConfig* p_stream_config)
{
  uint32_t len = 0;
  uint8_t  i;

  for (i = 0; i < p_stream_config->rx_framelength_type_size_; ++i)
  {
    if (i == 0)
    {
      len = p_stream_config->rx_frame_[p_stream_config->rx_framelength_pos_];
    }
    else
    {
      len <<= 8;
      len |= p_stream_config->rx_frame_[p_stream_config->rx_framelength_pos_ + i];
    }
  }

  len += p_stream_config->rx_framelength_offset_;
  return len;
}


/**
 * @brief  DS_StreamConfig構造体の初期化
 *
 *         DS_StreamConfig構造体を初期化し，デフォルト値で埋める．
 * @param  p_stream_config  DriverSuper構造体のDS_StreamConfig
 * @return DS_ERR_CODE
 */
static DS_ERR_CODE DS_reset_stream_config_(DS_StreamConfig* p_stream_config)
{
  p_stream_config->is_enabled_ = 0;
  p_stream_config->is_strict_frame_search_ = 0;

  p_stream_config->general_cmd_tx_count_               = 0;
  p_stream_config->req_tlm_cmd_tx_count_               = 0;
  p_stream_config->req_tlm_cmd_tx_count_after_last_tx_ = 0;
  p_stream_config->rx_frame_fix_count_                 = 0;

  p_stream_config->general_cmd_tx_time_ = TMGR_get_master_clock();
  p_stream_config->req_tlm_cmd_tx_time_ = TMGR_get_master_clock();
  p_stream_config->rx_frame_fix_time_   = TMGR_get_master_clock();

  p_stream_config->tx_frame_       = NULL;
  p_stream_config->tx_frame_size_  = 0;
  p_stream_config->rx_header_      = NULL;
  p_stream_config->rx_header_size_ = 0;
  p_stream_config->rx_footer_      = NULL;
  p_stream_config->rx_footer_size_ = 0;
  p_stream_config->rx_frame_size_  = 0;

  p_stream_config->rx_framelength_pos_       = -1;
  p_stream_config->rx_framelength_type_size_ = 0;
  p_stream_config->rx_framelength_offset_    = 0;

  p_stream_config->data_analyzer_ = DS_data_analyzer_dummy_;
  p_stream_config->ret_from_data_analyzer_ = DS_ERR_CODE_OK;

  p_stream_config->should_monitor_for_tlm_disruption_ = 0;
  p_stream_config->time_threshold_for_tlm_disruption_ = 60 * 1000;      // この値はよく考えること

  p_stream_config->is_validation_needed_for_send_ = 0;
  p_stream_config->is_validation_needed_for_rec_  = 0;

  p_stream_config->rx_frame_rec_len_ = 0;
  p_stream_config->rx_frame_head_pos_of_frame_candidate_ = 0;

  memset(p_stream_config->rx_frame_,
         0x00,
         sizeof(p_stream_config->rx_frame_));

  p_stream_config->is_rx_buffer_carry_over_ = 0;
  p_stream_config->carry_over_buffer_size_ = 0;
  p_stream_config->carry_over_buffer_next_pos_ = 0;
  memset(p_stream_config->rx_buffer_for_carry_over_,
         0x00,
         sizeof(p_stream_config->rx_buffer_for_carry_over_));

  // DS_StreamSendStatus の初期化
  p_stream_config->send_status_.status_code    = DS_STREAM_SEND_STATUS_DISABLE;
  p_stream_config->send_status_.ret_from_if_tx = 0;

  // DS_StreamRecStatus の初期化
  p_stream_config->rec_status_.status_code                  = DS_STREAM_REC_STATUS_DISABLE;
  p_stream_config->rec_status_.fixed_frame_len              = 0;
  p_stream_config->rec_status_.tlm_disruption_status        = DS_STREAM_TLM_DISRUPTION_STATUS_OK;
  p_stream_config->rec_status_.count_of_carry_over_failures = 0;

  return DS_ERR_CODE_OK;
}


static DS_ERR_CODE DS_validate_stream_config_(DS_StreamConfig* p_stream_config)
{
  if (!p_stream_config->is_enabled_) return DS_ERR_CODE_OK;

  if (p_stream_config->tx_frame_size_  != 0 && p_stream_config->tx_frame_  == NULL) return DS_ERR_CODE_ERR;
  if (p_stream_config->rx_header_size_ != 0 && p_stream_config->rx_header_ == NULL) return DS_ERR_CODE_ERR;
  if (p_stream_config->rx_footer_size_ != 0 && p_stream_config->rx_footer_ == NULL) return DS_ERR_CODE_ERR;

  if (p_stream_config->rx_frame_size_ > DS_RX_FRAME_SIZE_MAX) return DS_ERR_CODE_ERR;   // [TODO] 現在はBigData未実装（詳細はヘッダファイル参照）のため，ここで弾く

  if (p_stream_config->rx_frame_size_ < 0)
  {
    // テレメトリ可変長
    if (p_stream_config->rx_framelength_pos_ < 0)
    {
      // フレームサイズデータがない場合
      // フッタの存在が必須
      if (p_stream_config->rx_footer_size_ == 0) return DS_ERR_CODE_ERR;
    }
    else
    {
      if (p_stream_config->rx_header_size_ == 0) return DS_ERR_CODE_ERR;       // 可変長かつヘッダなしは対応しない（固定長のようにして回避する．詳細はヘッダファイル参照）
      if (p_stream_config->rx_framelength_pos_ < p_stream_config->rx_header_size_) return DS_ERR_CODE_ERR;    // フレームサイズがヘッダ（つまり固定値）に含まれることはありえないので
      if (!(p_stream_config->rx_framelength_type_size_ == 1 ||
            p_stream_config->rx_framelength_type_size_ == 2 ||
            p_stream_config->rx_framelength_type_size_ == 3 ||
            p_stream_config->rx_framelength_type_size_ == 4 )) return DS_ERR_CODE_ERR;    // 現在はuint8 to uint32のみ対応
    }
  }
  else if (p_stream_config->rx_frame_size_ == 0)
  {
    // テレメなし
  }
  else
  {
    // テレメトリ固定長
    if (p_stream_config->rx_frame_size_ < (p_stream_config->rx_header_size_ + p_stream_config->rx_footer_size_)) return DS_ERR_CODE_ERR;
  }

  if (p_stream_config->is_strict_frame_search_)
  {
    // ヘッダがあることが前提
    if (p_stream_config->rx_header_size_ == 0) return DS_ERR_CODE_ERR;
  }

  p_stream_config->is_validation_needed_for_send_ = 0;
  p_stream_config->is_validation_needed_for_rec_ = 0;
  return DS_ERR_CODE_OK;
}


static DS_ERR_CODE DS_load_init_setting_dummy_(DriverSuper* p_super)
{
  (void)p_super;
  return DS_ERR_CODE_OK;
}

static DS_ERR_CODE DS_data_analyzer_dummy_(DS_StreamConfig* p_stream_config, void* p_driver)
{
  (void)p_stream_config;
  (void)p_driver;
  return DS_ERR_CODE_OK;
}


// ###### DS_Config Getter/Setter ######
// FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている関数がいくつかある
const DS_RecStatus* DSC_get_rec_status(const DriverSuper* p_super)
{
  return &p_super->config.rec_status_;
}

uint32_t DSC_get_rx_count(const DriverSuper* p_super)
{
  return (uint32_t)p_super->config.rx_count_;
}

uint32_t DSC_get_rx_call_count(const DriverSuper* p_super)
{
  return (uint32_t)p_super->config.rx_call_count_;
}

const ObcTime* DSC_get_rx_time(const DriverSuper* p_super)
{
  return &p_super->config.rx_time_;
}

uint8_t DSC_get_should_monitor_for_rx_disruption(const DriverSuper* p_super)
{
  return (uint8_t)p_super->config.should_monitor_for_rx_disruption_;
}

void DSC_enable_monitor_for_rx_disruption(DriverSuper* p_super)
{
  p_super->config.should_monitor_for_rx_disruption_ = 1;
}

void DSC_disable_monitor_for_rx_disruption(DriverSuper* p_super)
{
  p_super->config.should_monitor_for_rx_disruption_ = 0;
}

uint32_t DSC_get_time_threshold_for_rx_disruption(const DriverSuper* p_super)
{
  return (uint32_t)p_super->config.time_threshold_for_rx_disruption_;
}

void DSC_set_time_threshold_for_rx_disruption(DriverSuper* p_super,
                                              const uint32_t time_threshold_for_rx_disruption)
{
  p_super->config.time_threshold_for_rx_disruption_ = time_threshold_for_rx_disruption;
}

DS_RX_DISRUPTION_STATUS_CODE DSC_get_rx_disruption_status(const DriverSuper* p_super)
{
  return (DS_RX_DISRUPTION_STATUS_CODE)p_super->config.rec_status_.rx_disruption_status;
}

// ###### DS_StreamConfig Getter/Setter ######
// FIXME: HEWでWarningが出てしまう（gccではでない）ので，キャストしている関数がいくつかある
uint8_t DSSC_get_is_enable(const DS_StreamConfig* p_stream_config)
{
  return (uint8_t)p_stream_config->is_enabled_;
}

void DSSC_enable(DS_StreamConfig* p_stream_config)
{
  p_stream_config->is_enabled_ = 1;
  p_stream_config->is_validation_needed_for_send_ = 1;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_disable(DS_StreamConfig* p_stream_config)
{
  p_stream_config->is_enabled_ = 0;
}

uint8_t DSSC_get_is_strict_frame_search(const DS_StreamConfig* p_stream_config)
{
  return (uint8_t)p_stream_config->is_strict_frame_search_;
}

void DSSC_enable_strict_frame_search(DS_StreamConfig* p_stream_config)
{
  p_stream_config->is_strict_frame_search_ = 1;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_disable_strict_frame_search(DS_StreamConfig* p_stream_config)
{
  p_stream_config->is_strict_frame_search_ = 0;
}

const DS_StreamSendStatus* DSSC_get_send_status(const DS_StreamConfig* p_stream_config)
{
  return &p_stream_config->send_status_;
}

const DS_StreamRecStatus* DSSC_get_rec_status(const DS_StreamConfig* p_stream_config)
{
  return &p_stream_config->rec_status_;
}

uint32_t DSSC_get_general_cmd_tx_count(const DS_StreamConfig* p_stream_config)
{
  return (uint32_t)p_stream_config->general_cmd_tx_count_;
}

uint32_t DSSC_get_req_tlm_cmd_tx_count(const DS_StreamConfig* p_stream_config)
{
  return (uint32_t)p_stream_config->req_tlm_cmd_tx_count_;
}

uint32_t DSSC_get_req_tlm_cmd_tx_count_after_last_tx(const DS_StreamConfig* p_stream_config)
{
  return (uint32_t)p_stream_config->req_tlm_cmd_tx_count_after_last_tx_;
}

uint32_t DSSC_get_rx_frame_fix_count(const DS_StreamConfig* p_stream_config)
{
  return (uint32_t)p_stream_config->rx_frame_fix_count_;
}

const ObcTime* DSSC_get_general_cmd_tx_time(const DS_StreamConfig* p_stream_config)
{
  return &p_stream_config->general_cmd_tx_time_;
}

const ObcTime* DSSC_get_req_tlm_cmd_tx_time(const DS_StreamConfig* p_stream_config)
{
  return &p_stream_config->req_tlm_cmd_tx_time_;
}

const ObcTime* DSSC_get_rx_frame_fix_time(const DS_StreamConfig* p_stream_config)
{
  return &p_stream_config->rx_frame_fix_time_;
}

void DSSC_set_tx_frame(DS_StreamConfig* p_stream_config,
                       uint8_t* tx_frame)
{
  p_stream_config->tx_frame_ = tx_frame;
  p_stream_config->is_validation_needed_for_send_ = 1;
}

void DSSC_set_tx_frame_size(DS_StreamConfig* p_stream_config,
                            const uint16_t tx_frame_size)
{
  p_stream_config->tx_frame_size_ = tx_frame_size;
  p_stream_config->is_validation_needed_for_send_ = 1;
}

uint16_t DSSC_get_tx_frame_size(const DS_StreamConfig* p_stream_config)
{
  return (uint16_t)p_stream_config->tx_frame_size_;
}

const uint8_t* DSSC_get_rx_frame(const DS_StreamConfig* p_stream_config)
{
  return p_stream_config->rx_frame_;
}

void DSSC_set_rx_header(DS_StreamConfig* p_stream_config,
                        const uint8_t* rx_header,
                        const uint16_t rx_header_size)
{
  p_stream_config->rx_header_ = rx_header;
  p_stream_config->rx_header_size_ = rx_header_size;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_set_rx_footer(DS_StreamConfig* p_stream_config,
                        const uint8_t* rx_footer,
                        const uint16_t rx_footer_size)
{
  p_stream_config->rx_footer_ = rx_footer;
  p_stream_config->rx_footer_size_ = rx_footer_size;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_set_rx_frame_size(DS_StreamConfig* p_stream_config,
                            const int16_t rx_frame_size)
{
  p_stream_config->rx_frame_size_ = rx_frame_size;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

uint16_t DSSC_get_rx_header_size(const DS_StreamConfig* p_stream_config)
{
  return (uint16_t)p_stream_config->rx_header_size_;
}

uint16_t DSSC_get_rx_footer_size(const DS_StreamConfig* p_stream_config)
{
  return (uint16_t)p_stream_config->rx_footer_size_;
}

int16_t DSSC_get_rx_frame_size(const DS_StreamConfig* p_stream_config)
{
  return (int16_t)p_stream_config->rx_frame_size_;
}

void DSSC_set_rx_framelength_pos(DS_StreamConfig* p_stream_config,
                                 const int16_t rx_framelength_pos)
{
  p_stream_config->rx_framelength_pos_ = rx_framelength_pos;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_set_rx_framelength_type_size(DS_StreamConfig* p_stream_config,
                                       const uint16_t rx_framelength_type_size)
{
  p_stream_config->rx_framelength_type_size_ = rx_framelength_type_size;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_set_rx_framelength_offset(DS_StreamConfig* p_stream_config,
                                    const uint16_t rx_framelength_offset)
{
  p_stream_config->rx_framelength_offset_ = rx_framelength_offset;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

uint8_t DSSC_get_should_monitor_for_tlm_disruption(const DS_StreamConfig* p_stream_config)
{
  return (uint8_t)p_stream_config->should_monitor_for_tlm_disruption_;
}

void DSSC_enable_monitor_for_tlm_disruption(DS_StreamConfig* p_stream_config)
{
  p_stream_config->should_monitor_for_tlm_disruption_ = 1;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

void DSSC_disable_monitor_for_tlm_disruption(DS_StreamConfig* p_stream_config)
{
  p_stream_config->should_monitor_for_tlm_disruption_ = 0;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

uint32_t DSSC_get_time_threshold_for_tlm_disruption(const DS_StreamConfig* p_stream_config)
{
  return (uint32_t)p_stream_config->time_threshold_for_tlm_disruption_;
}

void DSSC_set_time_threshold_for_tlm_disruption(DS_StreamConfig* p_stream_config,
                                                const uint32_t time_threshold_for_tlm_disruption)
{
  p_stream_config->time_threshold_for_tlm_disruption_ = time_threshold_for_tlm_disruption;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

DS_STREAM_TLM_DISRUPTION_STATUS_CODE DSSC_get_tlm_disruption_status(const DS_StreamConfig* p_stream_config)
{
  return (DS_STREAM_TLM_DISRUPTION_STATUS_CODE)p_stream_config->rec_status_.tlm_disruption_status;
}

void DSSC_set_data_analyzer(DS_StreamConfig* p_stream_config,
                            DS_ERR_CODE (*data_analyzer)(DS_StreamConfig* p_stream_config, void* p_driver))
{
  p_stream_config->data_analyzer_ = data_analyzer;
  p_stream_config->is_validation_needed_for_rec_ = 1;
}

DS_ERR_CODE DSSC_get_ret_from_data_analyzer(const DS_StreamConfig* p_stream_config)
{
  return (DS_ERR_CODE)p_stream_config->ret_from_data_analyzer_;
}


// ###### Driver汎用Util関数 ######

CCP_EXEC_STS DS_conv_driver_err_to_ccp_exec_sts(DS_DRIVER_ERR_CODE code)
{
  switch (code)
  {
  case DS_DRIVER_ERR_CODE_ILLEGAL_CONTEXT:
  case DS_DRIVER_ERR_CODE_UNKNOWN_ERR:
    // 全てこれでいいのかは，要検討
    return CCP_EXEC_ILLEGAL_CONTEXT;
  case DS_DRIVER_ERR_CODE_ILLEGAL_PARAMETER:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case DS_DRIVER_ERR_CODE_ILLEGAL_LENGTH:
    return CCP_EXEC_ILLEGAL_LENGTH;
  default:
    // ここに来るのは以下
    // DS_DRIVER_ERR_CODE_OK
    return CCP_EXEC_SUCCESS;
  }
}


CCP_EXEC_STS DS_conv_cmd_err_to_ccp_exec_sts(DS_CMD_ERR_CODE code)
{
  switch (code)
  {
  case DS_CMD_ILLEGAL_CONTEXT:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  case DS_CMD_ILLEGAL_PARAMETER:
    return CCP_EXEC_ILLEGAL_PARAMETER;
  case DS_CMD_ILLEGAL_LENGTH:
    return CCP_EXEC_ILLEGAL_LENGTH;
  default:
    // ここに来るのは以下の３つ
    // DS_CMD_OK
    // DS_CMD_DRIVER_SUPER_ERR
    // DS_CMD_UNKNOWN_ERR
    // 下２つのエラーはDriver側の問題で，そちらでエラー情報を持つべき
    // ここでは SUCCESSを返す
    return CCP_EXEC_SUCCESS;
  }
}

#pragma section
