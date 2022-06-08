cmake_minimum_required(VERSION 3.13)

project(C2A_USER_IF_WRAPPER)

set(C2A_SRCS
  if_list.c
)

# 通常，S2EではC++ビルドされるが，C2A core開発のため，C2A単体をC89でライブラリビルドする場合は，mockupをビルド対象にする
if(USE_SILS_MOCKUP)
  message("USE SILS_MOCKUP")

  #target_sources(${PROJECT_NAME} PRIVATE
  list(APPEND C2A_SRCS
    SilsMockup/ccsds_sils.c
    SilsMockup/uart_sils.c
    SilsMockup/wdt_sils.c
  )
else()
  #target_sources(${PROJECT_NAME} PUBLIC
  list(APPEND C2A_SRCS
    Sils/ccsds_sils.cpp
    Sils/uart_sils.cpp
    Sils/wdt_sils.cpp
  )
endif()

if(USE_SCI_COM_WINGS AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_WINGS)
  #target_sources(${PROJECT_NAME} PUBLIC
  list(APPEND C2A_SRCS
    Sils/sils_sci_if.cpp
    Sils/ccsds_sils_sci_if.cpp
  )
  message("USE SCI_COM_WINGS")
endif()

if(USE_SCI_COM_UART AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_UART)
  #target_sources(${PROJECT_NAME} PUBLIC
  list(APPEND C2A_SRCS
    Sils/sils_sci_if.cpp
    Sils/uart_sils_sci_if.cpp
  )
  message("USE SCI_COM_UART")
endif()

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
