## CMake file for S2E integration

cmake_minimum_required(VERSION 3.10)
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

include_directories(src)
add_definitions(-DSILS_FW)

add_subdirectory(src/src_core)

add_subdirectory(src/src_user/Applications)
add_subdirectory(src/src_user/CmdTlm)
add_subdirectory(src/src_user/Drivers)
add_subdirectory(src/src_user/IfWrapper)
add_subdirectory(src/src_user/Library)
add_subdirectory(src/src_user/Settings)

set(C2A_SRCS
  src/src_user/c2a_main.c
)

if(BUILD_C2A_AS_CXX)
  message("Build as C++!!!")
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})


target_link_libraries(${PROJECT_NAME} C2A_CORE
  C2A_USER_APPS
  C2A_USER_CMD_TLM
  C2A_USER_DRIVERS
  C2A_USER_IF_WRAPPER
  C2A_USER_LIB
  C2A_USER_SETTINGS
)



# Output debug print to SILS console window
option(SHOW_DEBUG_PRINT_ON_SILS "Show debug print")
set(SHOW_DEBUG_PRINT_ON_SILS ON)
if(SHOW_DEBUG_PRINT_ON_SILS)
  add_definitions(-DSHOW_DEBUG_PRINT_ON_SILS)
  message("Show debug print")
endif()

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND git_revision.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/Script)
else()
  add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ./git_revision.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/Script)
endif()

include(../../common.cmake)

# Compile option
if(NOT MSVC)
  # disable warning to main()
  target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-main")

  if(ADD_WERROR_FLAGS)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Werror")
  endif()
  if(ADD_WEXTRA_FLAGS)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wextra")
  endif()
endif()
