#pragma section REPRO
/**
 * @file
 * @brief   ccsds_sils_sci_if
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
 */

#include "ccsds_sils_sci_if.hpp"

int SILS_SIC_IF_init(void)
{
  return 0;
}

int SILS_SIC_IF_TX(unsigned char* data_v, int count)
{
  SILS_SCI_IF_sci_com_.Send(data_v, 0, count);
  return 0;
}

int SILS_SIC_IF_RX(unsigned char* data_v, int count)
{
  return SILS_SCI_IF_sci_com_.Receive(data_v, 0, count);
}

#pragma section
