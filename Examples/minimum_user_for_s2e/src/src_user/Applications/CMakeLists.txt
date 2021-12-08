cmake_minimum_required(VERSION 3.13)

project(C2A_USER_APPS)

set(C2A_SRCS
  app_registry.c
  DriverInstances/di_gs.c
  DriverInstances/di_uart_test.c
  UserDefined/debug_apps.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
