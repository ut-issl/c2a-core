#pragma section REPRO
/**
 * @file
 * @brief   sils_sci_ccsds_if
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
 *          Windows上でcom0comを使うことを想定
 *          SCIComPort classは基本的にEQU ZEUSのコードを流用
 */

#include "sils_sci_ccsds_if.hpp"


// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
#ifdef _WIN32
static SCIComPortCcsds SILS_SCI_CCSDS_IF_sci_com_(11);
#else
static SCIComPortCcsds SILS_SCI_CCSDS_IF_sci_com_(1);
#endif

int SILS_SCI_CCSDS_IF_init(void)
{
  return 0;
}

int SILS_SCI_CCSDS_IF_TX(unsigned char* data_v, int count)
{
  SILS_SCI_CCSDS_IF_sci_com_.Send(data_v, 0, count);
  return 0;
}

int SILS_SCI_CCSDS_IF_RX(unsigned char* data_v, int count)
{
  return SILS_SCI_CCSDS_IF_sci_com_.Receive(data_v, 0, count);
}

#pragma section
