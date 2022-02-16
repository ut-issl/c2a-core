#ifndef COMMAND_DISPATCHER_H_
#define COMMAND_DISPATCHER_H_

#include "../System/TimeManager/obc_time.h"
#include "common_cmd_packet.h"
#include "packet_list.h"
#include <src_user/TlmCmd/command_definitions.h>

typedef struct
{
  ObcTime time;
  CMD_CODE code;
  CCP_EXEC_STS sts;
} CDIS_ExecInfo;

typedef struct
{
  CDIS_ExecInfo prev;
  CDIS_ExecInfo prev_err;
  unsigned int error_counter;
  int lockout;
  int stop_on_error;
  PacketList* pli;
} CommandDispatcher;

CommandDispatcher CDIS_init(PacketList* pli);

void CDIS_dispatch_command(CommandDispatcher* cdis);

void CDIS_clear_command_list(CommandDispatcher* cdis);

void CDIS_clear_error_status(CommandDispatcher* cdis);

#endif
