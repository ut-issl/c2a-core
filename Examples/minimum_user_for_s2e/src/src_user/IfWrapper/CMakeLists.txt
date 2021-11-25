cmake_minimum_required(VERSION 3.10)

project(C2A_USER_IF_WRAPPER)

add_library(${PROJECT_NAME} STATIC
  if_list.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89

# 通常，S2EではC++ビルドされるが，C2A core開発のため，C2A単体をC89でライブラリビルドする場合は，mockupをビルド対象にする
if(USE_SILS_MOCKUP)
  target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/SilsMockup/ccsds_sils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/SilsMockup/uart_sils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/SilsMockup/wdt_sils.c
  )
  message("USE SILS_MOCKUP")
else()
  message(${CMAKE_CURRENT_SOURCE_DIR})
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Sils/ccsds_sils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sils/uart_sils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sils/wdt_sils.cpp
  )
endif()

if(USE_SCI_COM_WINGS AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_WINGS)
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Sils/ccsds_sils_sci_if.cpp
  )
  message("USE SCI_COM_WINGS")
endif()

if(USE_SCI_COM_UART AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_UART)
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Sils/uart_sils_sci_if.cpp
  )
  message("USE SCI_COM_UART")
endif()
