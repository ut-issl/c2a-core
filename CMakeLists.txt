cmake_minimum_required(VERSION 3.10)

project(C2A_CORE)

# user config
option(USE_C2A_CORE_APPS    "use C2A-core Applications" ON)
option(USE_C2A_CORE_LIB     "use C2A-core Library" ON)

set(C2A_SRCS
  # src/src_core/c2a_core_main.c
  CmdTlm/block_command_loader.c
  CmdTlm/block_command_table.c
  CmdTlm/block_command_executor.c
  CmdTlm/command_analyze.c
  CmdTlm/command_dispatcher.c
  CmdTlm/common_tlm_cmd_packet_util.c
  CmdTlm/packet_handler.c
  CmdTlm/packet_list.c
  CmdTlm/telemetry_frame.c
  CmdTlm/telemetry_generator.c
  Drivers/Super/driver_super.c
  Drivers/Super/driver_super_issl_format.c
  System/AnomalyLogger/anomaly_logger.c
  System/ApplicationManager/app_info.c
  System/ApplicationManager/app_manager.c
  System/EventManager/event_manager.c
  System/EventManager/event_logger.c
  System/EventManager/event_handler.c
  System/ModeManager/mode_manager.c
  System/TaskManager/task_dispatcher.c
  System/TimeManager/obc_time.c
  System/TimeManager/time_manager.c
  System/WatchdogTimer/watchdog_timer.c
)
add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

add_subdirectory(Applications)
add_subdirectory(Library)

if(USE_C2A_CORE_APPS)
  target_link_libraries(${PROJECT_NAME} C2A_CORE_APPS)
endif()

if(USE_C2A_CORE_LIB)
  target_link_libraries(${PROJECT_NAME} C2A_CORE_LIB)
endif()

# Compile option
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
    target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  #target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragma")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragmas") # SJIS, 32bit
endif()

include(common.cmake)
