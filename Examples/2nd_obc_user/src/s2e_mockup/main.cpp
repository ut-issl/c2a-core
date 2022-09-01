#include "src_core/c2a_core_main.h"
#include "src_core/System/TaskManager/task_dispatcher.h"
#include "src_core/System/TimeManager/time_manager.h"
#include "src_core/System/WatchdogTimer/watchdog_timer.h"

int main(){
  TMGR_init();                //Time Manager

  C2A_core_init();

  WDT_init();                 // WDT

  TMGR_clear();

  // OBC_C2A::MainRoutine()
  for(;;){
    TMGR_count_up_master_clock();
    TDSP_execute_pl_as_task_list();
  }
}

int OBC_C2A_SendFromObc(int port_id, unsigned char* buffer, int offset, int count){ return 0; }
int OBC_C2A_ReceivedByObc(int port_id, unsigned char* buffer, int offset, int count){ return 0; }
