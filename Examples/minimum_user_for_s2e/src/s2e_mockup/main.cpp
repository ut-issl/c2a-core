#include "command_analyze.h"
#include "telemetry_frame.h"
#include "packet_handler.h"
#include "anomaly_logger.h"
#include "app_manager.h"
#include "app_registry.h"
#include "time_manager.h"
#include "block_command_table.h"
#include "mode_manager.h"
#include "task_dispatcher.h"
#include "watchdog_timer.h"

int main(){
  CA_initialize();            //Cmd Analyze
  TF_initialize();            //TLM frame
  PH_init();                  //Packet Handler
  TMGR_init();                //Time Manager
  AL_initialize();            //Anomaly Logger
  AM_initialize();            //App Manager
  AR_load_initial_settings();	//App Registry
  AM_initialize_all_apps();	  //App Managerに登録されてるアプリの初期化
  BCT_initialize();	          //Block Cmd Table
  MM_initialize();            //Mode Manager
  TDSP_initialize();          //Task Dispatcher
  WDT_init();                 // WDT
}
