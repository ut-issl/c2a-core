cmake_minimum_required(VERSION 3.10)

project(C2A_USER_LIB)

set(C2A_SRCS
  git_revision.c
  print.c
  VT100.c
)
add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

include(../../src_core/common.cmake)
