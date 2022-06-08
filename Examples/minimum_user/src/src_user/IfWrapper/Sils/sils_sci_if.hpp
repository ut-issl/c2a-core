/**
 * @file
 * @brief   sils_sci_if
 * @details SCI COMでやりとりするIF
 */
#ifndef SILS_SCI_IF_HPP_
#define SILS_SCI_IF_HPP_

#ifdef WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <stddef.h>
#include <stdio.h>

class SCIComPort
{
public:
  SCIComPort(int port);
  virtual ~SCIComPort(void);

  int Send(unsigned char* buffer, size_t length, size_t offset);
  int Receive(unsigned char* buffer, size_t length, size_t offset);

private:
#ifdef WIN32
  HANDLE myHComPort_;
  DCB config_;
#else
  int myHComPort_;
  struct termios config_;
#endif
  bool init_success;
};

#endif

