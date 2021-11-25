cmake_minimum_required(VERSION 3.10)

project(C2A_CORE_APPS)

add_library(${PROJECT_NAME} STATIC
  anomaly_handler.c
  divided_cmd_utility.c
  gs_command_dispatcher.c
  event_utility.c
  memory_dump.c
  nop.c
  realtime_command_dispatcher.c
  timeline_command_dispatcher.c
  utility_command.c
  utility_counter.c
  telemetry_manager.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89

# Compile option
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
    target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  #target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragma")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragmas") # SJIS, 32bit
endif()
