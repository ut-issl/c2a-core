/**
 * @file
 * @brief   ccsds_sils_sci_if
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
 */
#ifndef CCSDS_SILS_SCI_IF_HPP_
#define CCSDS_SILS_SCI_IF_HPP_

#include <stddef.h>
#include "sils_sci_if.hpp"

// 最初だけ初期化して、プログラム終了時にポートを閉じるようにしたい
#ifdef WIN32
static SCIComPort SILS_SCI_IF_sci_com_(11);
#else
static SCIComPort SILS_SCI_IF_sci_com_(1);
#endif

int SILS_SIC_IF_init();
int SILS_SIC_IF_TX(unsigned char* data_v, int count);
int SILS_SIC_IF_RX(unsigned char* data_v, int count);

#endif
