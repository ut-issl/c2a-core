#pragma section REPRO
/**
 * @file
 * @brief   sils_sci_if
 * @details SCI COMでやりとりするIF
 */

#include "sils_sci_if.hpp"

#ifdef WIN32
SCIComPort::SCIComPort(int port)
{
  char port_settings[15];
  snprintf(port_settings, 15, "%s%d", "\\\\.\\COM", port);
  myHComPort_ = CreateFile(port_settings, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if ((int)myHComPort_ == -1)
  {
    // 正常にポートオープンできていない場合は終了
    CloseHandle(myHComPort_);
    init_success = false;
    return;
  }

  // どうやら正常ポートopenにならないっぽく，これが必要
  init_success = true;

  // ポートのボーレート、パリティ等を設定
  config_.BaudRate = 115200;
  config_.Parity = PARITY_NONE;
  config_.ByteSize = 8;
  config_.StopBits = STOPBITS_10;

  // Parity、StopBits、DataBitsも同様に設定
  SetCommState(myHComPort_, &config_);
}

SCIComPort::~SCIComPort(void)
{
  if (init_success == true)
  {
    CloseHandle(myHComPort_);
  }
}

int SCIComPort::Send(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD toWriteBytes = count; // 送信したいバイト数
  DWORD writeBytes;           // 実際に送信されたバイト数

  if (init_success == true)
  {
    WriteFile(myHComPort_, buffer + offset, toWriteBytes, &writeBytes, NULL);
    return writeBytes;
  }
  else
  {
    return 0;
  }
}

int SCIComPort::Receive(unsigned char* buffer, size_t offset, size_t count)
{
  DWORD fromReadBytes = count; // 受信したいバイト数
  DWORD dwErrors;
  COMSTAT ComStat;
  DWORD dwCount;               // 受信したバイト数

  if (init_success == true)
  {
    ClearCommError(myHComPort_, &dwErrors, &ComStat);
    dwCount = ComStat.cbInQue;

    if (dwCount > 0)
    {
      if (dwCount < count)
      {
        fromReadBytes = dwCount;
        ReadFile(myHComPort_, buffer + offset, fromReadBytes, &dwCount, NULL);
      }
      else
      {
        fromReadBytes = count; // 読み込みすぎるとデータが失われるので読み込む量を制御
        ReadFile(myHComPort_, buffer + offset, fromReadBytes, &dwCount, NULL);
      }
    }

    return dwCount;
  }
  else
  {
    return 0;
  }
}

#else

SCIComPort::SCIComPort(int port)
{
  char port_settings[13];
  snprintf(port_settings, 13, "%s%d", "/dev/tnt", port);
  if ((myHComPort_ = open(port_settings, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0)
  {
    close(myHComPort_);
    init_success = false;
    return;
  }

  // どうやら正常ポートopenにならないっぽく，これが必要
  init_success = true;

  cfsetispeed(&config_, 115200);
  cfsetospeed(&config_, 115200);
  config_.c_cflag     &=  ~PARENB;            // No Parity
  config_.c_cflag     &=  ~CSTOPB;            // 1 Stop Bit
  config_.c_cflag     &=  ~CSIZE;
  config_.c_cflag     |=  CS8;                // 8 Bits
  tcsetattr(myHComPort_, TCSANOW, &config_);
}

SCIComPort::~SCIComPort(void)
{
  if (init_success == true)
  {
    close(myHComPort_);
  }
}

int SCIComPort::Send(unsigned char* buffer, size_t offset, size_t count)
{
  unsigned long toWriteBytes = count; // 送信したいバイト数
  unsigned long writeBytes;           // 実際に送信されたバイト数

  if (init_success == true)
  {
    writeBytes = write(myHComPort_, buffer + offset, toWriteBytes);
    return writeBytes;
  }
  else
  {
    return 0;
  }
}

int SCIComPort::Receive(unsigned char* buffer, size_t offset, size_t count)
{
  unsigned long fromReadBytes = count; // 受信したいバイト数
  unsigned long dwErrors;
  unsigned long ComStat_cbInQue;
  unsigned long dwCount;               // 受信したバイト数

  if (init_success == true)
  {
    dwCount = ComStat_cbInQue;

    if (dwCount > 0)
    {
      if (dwCount < count)
      {
        fromReadBytes = dwCount;
        dwCount = read(myHComPort_, buffer + offset, fromReadBytes);
      }
      else
      {
        fromReadBytes = count; // 読み込みすぎるとデータが失われるので読み込む量を制御
        dwCount = read(myHComPort_, buffer + offset, fromReadBytes);
      }
    }

    return dwCount;
  }
  else
  {
    return 0;
  }
}

#endif

#pragma section

