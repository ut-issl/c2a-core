## CMake file for S2E integration

cmake_minimum_required(VERSION 3.13)
project(C2A)

# options
# SCI COM for connection to WINGS TMTC IF
# ！！！注意！！！
# これをONにした状態で，SCIの受け口がない場合（TMTC IFが動いてない状態）
# そちらのバッファが詰まってSILSの動作が止まることがあるので注意すること！
option(USE_SCI_COM_WINGS "Use SCI_COM_WINGS")

# SCI COM for connection to PC UART
# ！！！注意！！！
# これをONにした状態で，SCIの受け口がない場合（受けてのTeratermが起動していない状態）
# そちらのバッファが詰まってSILSの動作が止まることがあるので注意すること！
option(USE_SCI_COM_UART "Use SCI_COM_UART")


option(USE_SILS_MOCKUP "Use SILS mockup for build C2A with minimal user in C89 only" OFF)

# default config
set(USE_SCI_COM_WINGS ON)
set(USE_SCI_COM_UART OFF)

if(USE_SILS_MOCKUP)
  set(BUILD_C2A_AS_CXX OFF)
endif()

if(BUILD_C2A_AS_CXX)
  message("build C2A as C++!")
endif()

set(C2A_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/src_core)
set(C2A_USER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user)

include_directories(src)
add_definitions(-DSILS_FW)

# Output debug print to SILS console window
option(SHOW_DEBUG_PRINT_ON_SILS "Show debug print")
set(SHOW_DEBUG_PRINT_ON_SILS ON)
if(SHOW_DEBUG_PRINT_ON_SILS)
  add_definitions(-DSHOW_DEBUG_PRINT_ON_SILS)
  message("Show debug print")
endif()

execute_process(
  COMMAND git log -1 --format=%H
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_USER
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_REVISION_C2A_USER_SHORT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_REVISION_C2A_USER=\"${GIT_REVISION_C2A_USER}\"")
add_definitions("-DGIT_REVISION_C2A_USER_SHORT=0x${GIT_REVISION_C2A_USER_SHORT}")

add_subdirectory(${C2A_CORE_DIR})

add_subdirectory(${C2A_USER_DIR}/Applications)
add_subdirectory(${C2A_USER_DIR}/Drivers)
add_subdirectory(${C2A_USER_DIR}/IfWrapper)
add_subdirectory(${C2A_USER_DIR}/Library)
add_subdirectory(${C2A_USER_DIR}/Settings)
add_subdirectory(${C2A_USER_DIR}/TlmCmd)

set(C2A_SRCS
  ${C2A_USER_DIR}/c2a_main.c
)

if(BUILD_C2A_AS_CXX)
  message("Build as C++!!!")
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

if(USE_SILS_MOCKUP)
  add_executable(${PROJECT_NAME} ${C2A_SRCS})
else()
  add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})
endif()

set(C2A_USER_MODULES
  C2A_USER_APPS
  C2A_USER_CMD_TLM
  C2A_USER_DRIVERS
  C2A_USER_IF_WRAPPER
  C2A_USER_LIB
  C2A_USER_SETTINGS
)

if(MSVC)
  target_link_options(${PROJECT_NAME} PRIVATE "/WHOLEARCHIVE")
  target_link_libraries(${PROJECT_NAME} PRIVATE
    C2A_CORE
    ${C2A_USER_MODULES}
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    -Wl,--whole-archive
    C2A_CORE
    -Wl,--no-whole-archive
    ${C2A_USER_MODULES}
    -lm
  )
endif()

include(${C2A_USER_DIR}/common.cmake)
