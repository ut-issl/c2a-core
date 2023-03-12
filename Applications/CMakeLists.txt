cmake_minimum_required(VERSION 3.13)

project(C2A_CORE_APPS)

set(C2A_SRCS
  divided_cmd_utility.c
  gs_command_dispatcher.c
  event_utility.c
  memory_dump.c
  nop.c
  realtime_command_dispatcher.c
  timeline_command_dispatcher.c
  utility_command.c
  utility_counter.c
  telemetry_manager.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_CORE_DIR}/common.cmake)
