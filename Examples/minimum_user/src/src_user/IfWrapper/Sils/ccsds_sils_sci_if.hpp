/**
 * @file
 * @brief   ccsds_sils_sci_if
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
 */
#ifndef CCSDS_SILS_SCI_IF_HPP_
#define CCSDS_SILS_SCI_IF_HPP_

#include <stddef.h>
#include "sils_sci_if.hpp"

class SCIComPortCcsds : public SCIComPort
{
public:
  SCIComPortCcsds(int port) : SCIComPort(port) {};
  ~SCIComPortCcsds(void) {};
};

int SILS_SIC_IF_init();
int SILS_SIC_IF_TX(unsigned char* data_v, int count);
int SILS_SIC_IF_RX(unsigned char* data_v, int count);

#endif
