cmake_minimum_required(VERSION 3.13)

project(C2A_CORE_TEST_APPS)

set(C2A_SRCS
  test_ccp_util.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_CORE_DIR}/common.cmake)
