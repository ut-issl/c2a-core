cmake_minimum_required(VERSION 3.13)

project(C2A_CORE)

# user config
option(USE_ALL_C2A_CORE_APPS    "use C2A-core all Applications" ON)
option(USE_ALL_C2A_CORE_LIB     "use C2A-core all Library" ON)
option(USE_32BIT_COMPILER       "use 32bit compiler" OFF)

set(C2A_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(C2A_SRCS
  c2a_core_main.c
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
  TlmCmd/block_command_loader.c
  TlmCmd/block_command_table.c
  TlmCmd/block_command_executor.c
  TlmCmd/command_analyze.c
  TlmCmd/command_dispatcher.c
  TlmCmd/common_cmd_packet_util.c
  TlmCmd/packet_handler.c
  TlmCmd/packet_list_util.c
  TlmCmd/packet_list.c
  TlmCmd/telemetry_frame.c
  TlmCmd/telemetry_generator.c
  TlmCmd/Ccsds/cmd_space_packet.c
  TlmCmd/Ccsds/space_packet.c
  TlmCmd/Ccsds/tlm_space_packet.c
)

execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_CORE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_CORE_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_REVISION_C2A_CORE=\"${GIT_REVISION_C2A_CORE}\"")
add_definitions("-DGIT_REVISION_C2A_CORE_SHORT=0x${GIT_REVISION_C2A_CORE_SHORT}")

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

if(USE_ALL_C2A_CORE_APPS)
  add_subdirectory(Applications)
  target_link_libraries(${PROJECT_NAME} C2A_CORE_APPS)
endif()

if(USE_ALL_C2A_CORE_LIB)
  add_subdirectory(Library)
  target_link_libraries(${PROJECT_NAME} C2A_CORE_LIB)
endif()

include(common.cmake)
