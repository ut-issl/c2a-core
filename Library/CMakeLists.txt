cmake_minimum_required(VERSION 3.13)

project(C2A_CORE_LIB)

if(C2A_USE_SIMPLE_LIBC)
  message("use simple libc!!!")
  set(C2A_LIBC_SRC
    libc/memchr.c
    libc/bsearch.c
  )
else()
  set(C2A_LIBC_SRC "")
endif()

set(C2A_SRCS
  ascii_conv.c
  c2a_round.c
  crc.c
  endian.c
  majority_vote_for3.c
  ${C2A_LIBC_SRC}
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_CORE_DIR}/common.cmake)
