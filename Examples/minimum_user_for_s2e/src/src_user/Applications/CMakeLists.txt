cmake_minimum_required(VERSION 3.10)

project(C2A_USER_APPS)

add_library(${PROJECT_NAME} STATIC
  app_registry.c
  DriverInstances/di_gs.c
  DriverInstances/di_uart_test.c
  UserDefined/debug_apps.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
