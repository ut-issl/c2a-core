#pragma section REPRO
/**
 * @file
 * @brief MOBC の DI． MOBC における GS との DI に相当する
 */

#include "di_mobc.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/Library/print.h>
#include "../../Settings/port_config.h"
#include "../../Settings/DriverSuper/driver_buffer_define.h"

static void DI_MOBC_init_(void);
static void DI_MOBC_update_(void);
static void DI_MOBC_rt_tlm_packet_handler_init_(void);
static void DI_MOBC_rt_tlm_packet_handler_(void);

static MOBC_Driver mobc_driver_;
const MOBC_Driver* const mobc_driver = &mobc_driver_;

// バッファ
static DS_StreamRecBuffer DI_MOBC_rx_buffer_;
static uint8_t DI_MOBC_rx_buffer_allocation_[DS_STREAM_REC_BUFFER_SIZE_DEFAULT];

static const uint8_t DI_MOBC_kRtTlmPhMaxNumOfProc_ = 4;       //!< 一度に送出する最大テレメ数


AppInfo DI_MOBC_update(void)
{
  return AI_create_app_info("update_MOBC", DI_MOBC_init_, DI_MOBC_update_);
}

static void DI_MOBC_init_(void)
{
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;

  ret1 = DS_init_stream_rec_buffer(&DI_MOBC_rx_buffer_,
                                   DI_MOBC_rx_buffer_allocation_,
                                   sizeof(DI_MOBC_rx_buffer_allocation_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("MOBC buffer init Failed ! %d \n", ret1);
  }

  ret2 = MOBC_init(&mobc_driver_, PORT_CH_UART_MOBC, &DI_MOBC_rx_buffer_);
  if (ret2 != DS_INIT_OK)
  {
    Printf("MOBC init Failed ! %d \n", ret2);
  }
}

static void DI_MOBC_update_(void)
{
  DS_REC_ERR_CODE ret;
  ret = MOBC_rec(&mobc_driver_);

  // TODO: 必要があればここに処理を
  (void)ret;
}


AppInfo DI_MOBC_rt_tlm_packet_handler(void)
{
  return AI_create_app_info("MOBC_rt_tlm_ph",
                            DI_MOBC_rt_tlm_packet_handler_init_,
                            DI_MOBC_rt_tlm_packet_handler_);
}

static void DI_MOBC_rt_tlm_packet_handler_init_(void)
{
  // なにもしない
}

static void DI_MOBC_rt_tlm_packet_handler_(void)
{
  uint8_t i;
  CommonTlmPacket packet;   // FIXME: これは static にする？
                            //        static のほうがコンパイル時にアドレスが確定して安全． Out of stack space を回避できる
                            //        一方でメモリ使用量は増える．
  mobc_driver_.info.c2a.send_tlm_err_code = DS_CMD_OK;

  for (i = 0; i < DI_MOBC_kRtTlmPhMaxNumOfProc_; i++)
  {
    DS_CMD_ERR_CODE ret;

    // TODO: ここは一部 MW に入れるべきなのかなぁ．．．？
    //       最近 C2A の MW の扱いが難しい．いっそなくすか？

    // TODO: PH_rt_tlm_list を DI から隠蔽する？それか何かしらの共用構造体でのインスタンスの一部にするか？
    //       そうすると， rt_tlm の他の tlm ができたときに共通化が容易

    if (PL_is_empty(&PH_rt_tlm_list))
    {
      // キューが空なら終了
      return;
    }

    // 送信するパケットを取得
    packet = *(const CommonTlmPacket*)PL_get_head(&PH_rt_tlm_list)->packet;

    // 送信したパケットを消去
    // 以後エラーが出ても，そのパケットは再送しないので，取り出したここで消してしまう．
    PL_drop_executed(&PH_rt_tlm_list);

    // FIXME: 現状，WINGS の問題から DUMP TLMは考えない．
    //        APID_AOBC_TLM 以外を弾いている．
    if (CTP_get_apid(&packet) != APID_AOBC_TLM)
    {
      // FIXME: アノマリいれる？ 最後のエラーは以下で保存されるので不要な気もする．
      //        AOBC のアノマリ基準は？

      // 最後に起きたエラーを保存する
      mobc_driver_.info.c2a.send_tlm_err_code = DS_CMD_ILLEGAL_PARAMETER;
      continue;
    }

    // TODO: TPC のヘッダを別途設定する必要はないか，ちゃんと確認する．多分ないと思うけど

    ret = MOBC_send(&mobc_driver_, &packet);
    if (ret != DS_CMD_OK)
    {
      // 最後に起きたエラーを保存する
      mobc_driver_.info.c2a.send_tlm_err_code = ret;
    }
  }
}

#pragma section
