cmake_minimum_required(VERSION 3.10)

project(C2A_USER_APPS)

set(C2A_SRCS
  app_registry.c
  DriverInstances/di_gs.c
  DriverInstances/di_uart_test.c
  UserDefined/debug_apps.c
)
add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

include(../../src_core/common.cmake)
