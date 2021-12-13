cmake_minimum_required(VERSION 3.13)

project(C2A_CORE_LIB)

set(C2A_SRCS
  ascii_conv.c
  c2a_round.c
  crc.c
  endian_conv.c
  endian_memcpy.c
  majority_vote_for3.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_CORE_DIR}/common.cmake)
