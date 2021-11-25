cmake_minimum_required(VERSION 3.10)

project(C2A)

add_library(C2A_CORE STATIC
  # src/src_core/c2a_core_main.c
  Applications/anomaly_handler.c
  Applications/divided_cmd_utility.c
  Applications/gs_command_dispatcher.c
  Applications/event_utility.c
  Applications/memory_dump.c
  Applications/nop.c
  Applications/realtime_command_dispatcher.c
  Applications/timeline_command_dispatcher.c
  Applications/utility_command.c
  Applications/utility_counter.c
  Applications/telemetry_manager.c
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
  Library/ascii_conv.c
  Library/crc.c
  Library/endian_conv.c
  Library/endian_memcpy.c
  Library/exor.c
  Library/majority_vote_for3.c
  Library/matrix.c
  Library/print.c
  Library/quaternion.c
  Library/round.c
  Library/sum.c
  Library/vector.c
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

set_target_properties(C2A_CORE PROPERTIES C_STANDARD 90) # C89

# Compile option
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
    target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  #target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragma")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragmas") # SJIS, 32bit
endif()
