/**
 * @file
 * @brief   sils_sci_ccsds_if
 * @details WINGS TMTC IFとCCSDSのTransfer FrameをSCI COMでやりとりするIF
 *          Windows上でcom0comを使うことを想定
 *          SCIComPort classは基本的にEQU ZEUSのコードを流用
 */
#ifndef SILS_SCI_CCSDS_IF_HPP_
#define SILS_SCI_CCSDS_IF_HPP_

#include <stddef.h>
#include "sils_sci_if.hpp"

class SCIComPortCcsds : public SCIComPort
{
public:
  SCIComPortCcsds(int port) : SCIComPort(port) {};
  ~SCIComPortCcsds(void) {};
};

int SILS_SCI_CCSDS_IF_init();
int SILS_SCI_CCSDS_IF_tx(unsigned char* data_v, int count);
int SILS_SCI_CCSDS_IF_rx(unsigned char* data_v, int count);

#endif
