cmake_minimum_required(VERSION 3.13)

project(C2A_CORE)

# user config
option(USE_ALL_C2A_CORE_APPS        "use C2A-core all Applications" ON)
option(USE_ALL_C2A_CORE_TEST_APPS   "use C2A-core all Test Applications" ON)
option(USE_ALL_C2A_CORE_LIB         "use C2A-core all Library" ON)
option(USE_32BIT_COMPILER           "use 32bit compiler" OFF)

option(C2A_USE_SIMPLE_LIBC          "use C2A-core hosted simple libc implementation" OFF)
option(C2A_USE_C99_STDINT           "use C99 standard stdint.h" ON)

option(BUILD_C2A_AS_SILS_FW         "build C2A as SILS firmware" ON)
option(BUILD_C2A_AS_C99             "build C2A as C99" OFF)
option(BUILD_C2A_AS_CXX             "build C2A as C++" OFF)
option(BUILD_C2A_AS_UTF8            "build C2A as UTF-8" ON)

set(C2A_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(C2A_SRCS
  c2a_core_main.c
  Drivers/Protocol/common_tlm_cmd_packet_for_driver_super.c
  Drivers/Protocol/eb90_frame_for_driver_super.c
  Drivers/Protocol/eb90_packet_for_driver_super.c
  Drivers/Super/driver_super.c
  IfWrapper/Common/i2c_common.c
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

if(NOT C2A_USE_C99_STDINT)
  message("Use stdint.h wrapper")
  include_directories(Library/stdint_wrapper)
endif()

execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_CORE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_CORE_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_REVISION_C2A_CORE=\"${GIT_REVISION_C2A_CORE}\"")
add_definitions("-DGIT_REVISION_C2A_CORE_SHORT=0x${GIT_REVISION_C2A_CORE_SHORT}")

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

if(USE_ALL_C2A_CORE_APPS)
  add_subdirectory(Applications)
  target_sources(${PROJECT_NAME} PUBLIC $<TARGET_OBJECTS:C2A_CORE_APPS>)
endif()

if(USE_ALL_C2A_CORE_TEST_APPS)
  add_subdirectory(Applications/TestApp)
  target_sources(${PROJECT_NAME} PUBLIC $<TARGET_OBJECTS:C2A_CORE_TEST_APPS>)
endif()

if(USE_ALL_C2A_CORE_LIB)
  add_subdirectory(Library)
  target_sources(${PROJECT_NAME} PUBLIC $<TARGET_OBJECTS:C2A_CORE_LIB>)
endif()

include(common.cmake)
