#pragma section REPRO
#include "nop.h"

#include <stddef.h> // for NULL

static void NOP_nop_(void);

AppInfo NOP_create_app(void)
{
  return create_app_info("nop", NULL, NOP_nop_);
}

static void NOP_nop_() {
  // no operation
}

CCP_EXEC_STS Cmd_NOP(const CTCP* packet)
{
  (void)packet;
  return CCP_EXEC_SUCCESS;
}

#pragma section
