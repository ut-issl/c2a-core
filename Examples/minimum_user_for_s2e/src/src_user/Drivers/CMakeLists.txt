cmake_minimum_required(VERSION 3.10)

project(C2A_USER_DRIVERS)

set(C2A_SRCS
  Com/gs.c
  Com/gs_validate.c
  Etc/UART_TEST.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

include(../../src_core/common.cmake)
