cmake_minimum_required(VERSION 3.10)

project(C2A_CORE_LIB)

set(C2A_SRCS
  ascii_conv.c
  crc.c
  endian_conv.c
  endian_memcpy.c
  exor.c
  majority_vote_for3.c
  matrix.c
  print.c
  quaternion.c
  c2a_round.c
  sum.c
  vector.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

include(../common.cmake)
