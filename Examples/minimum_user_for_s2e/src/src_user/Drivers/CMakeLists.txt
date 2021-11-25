cmake_minimum_required(VERSION 3.10)

project(C2A_USER_DRIVERS)

add_library(${PROJECT_NAME} STATIC
  Com/gs.c
  Com/gs_validate.c
  Etc/UART_TEST.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
