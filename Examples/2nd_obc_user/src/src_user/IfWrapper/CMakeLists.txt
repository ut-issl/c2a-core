cmake_minimum_required(VERSION 3.13)

project(C2A_USER_IF_WRAPPER)

set(C2A_COMMON_SRCS
  if_list.c
)

# 通常，S2EではC++ビルドされるが，C2A core開発のため，C2A単体をC89でライブラリビルドする場合は，mockupをビルド対象にする
if(USE_SILS_MOCKUP)
  message("USE SILS_MOCKUP")

  #target_sources(${PROJECT_NAME} PRIVATE
  set(C2A_IF_IMPL_SRCS
    SilsMockup/i2c_sils.c
    SilsMockup/uart_sils.c
    SilsMockup/wdt_sils.c
  )
else()
  #target_sources(${PROJECT_NAME} PUBLIC
  set(C2A_IF_IMPL_COMMON_SRCS
    Sils/i2c_sils.cpp  
    Sils/uart_sils.cpp
    Sils/wdt_sils.cpp
  )

  if(USE_SCI_COM_WINGS) # TODO: これ USE_SCI_COM_UART では？
    add_definitions(-DUSE_SCI_COM_WINGS)
    #target_sources(${PROJECT_NAME} PUBLIC
    set(C2A_IF_COM_UART_SRCS
      Sils/uart_sils_sci_if.cpp
    )
    message("USE SCI_COM_UART")
  endif()

  set(C2A_IF_IMPL_SRCS
    ${C2A_IF_IMPL_COMMON_SRCS}
    ${C2A_IF_COM_UART_SRCS}
  )
endif()

set(C2A_SRCS
  ${C2A_COMMON_SRCS}
  ${C2A_IF_IMPL_SRCS}
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
