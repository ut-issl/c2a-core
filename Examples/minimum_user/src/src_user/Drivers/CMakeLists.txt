cmake_minimum_required(VERSION 3.13)

project(C2A_USER_DRIVERS)

set(C2A_SRCS
  Aocs/aobc_telemetry_buffer.c
  Aocs/aobc.c
  Com/gs.c
  Com/gs_validate.c
  Etc/uart_test.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
