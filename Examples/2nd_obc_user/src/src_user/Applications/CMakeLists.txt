cmake_minimum_required(VERSION 3.13)

project(C2A_USER_APPS)

set(C2A_SRCS
  app_registry.c
  DriverInstances/di_mobc.c
  UserDefined/debug_apps.c
  ${C2A_CORE_DIR}/Applications/gs_command_dispatcher.c
  ${C2A_CORE_DIR}/Applications/event_utility.c
  ${C2A_CORE_DIR}/Applications/nop.c
  ${C2A_CORE_DIR}/Applications/realtime_command_dispatcher.c
  ${C2A_CORE_DIR}/Applications/timeline_command_dispatcher.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
