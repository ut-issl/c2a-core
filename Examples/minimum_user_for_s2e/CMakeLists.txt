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

# default config
set(USE_SCI_COM_WINGS ON)
set(USE_SCI_COM_UART OFF)

include_directories(src)
add_definitions(-DSILS_FW)

add_subdirectory(src/src_core)

add_subdirectory(src/src_user/Applications)
add_subdirectory(src/src_user/CmdTlm)
add_subdirectory(src/src_user/Drivers)
add_subdirectory(src/src_user/IfWrapper)
add_subdirectory(src/src_user/Library)
add_subdirectory(src/src_user/Settings)

add_library(${PROJECT_NAME} STATIC
  src/src_user/c2a_main.c
)
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

if(USE_SILS_MOCKUP)
  set_source_files_properties(${C2A_SOURCE_FILES} PROPERTIES C_STANDARD 90)  # C89/C90
else()
  set_source_files_properties(${C2A_SOURCE_FILES} PROPERTIES LANGUAGE CXX)
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

# Compile option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragma")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragmas") # SJIS, 32bit
  if(ADD_WERROR_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
  endif()
  if(ADD_WEXTRA_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
  endif()
endif()
