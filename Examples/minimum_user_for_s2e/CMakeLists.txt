## CMake file for S2E integration

cmake_minimum_required(VERSION 3.10)
project(C2A)

include_directories(src)
add_definitions(-DSILS_FW)

add_subdirectory(src/src_core)

add_library(${PROJECT_NAME} STATIC
  # src/src_user/c2a_main.c
  src/src_user/Applications/app_registry.c
  src/src_user/Applications/DriverInstances/di_gs.c
  src/src_user/Applications/DriverInstances/di_uart_test.c
  src/src_user/Applications/UserDefined/debug_apps.c
  src/src_user/CmdTlm/block_command_definitions.c
  src/src_user/CmdTlm/block_command_user_settings.c
  src/src_user/CmdTlm/command_definitions.c
  src/src_user/CmdTlm/common_tlm_cmd_packet.c
  src/src_user/CmdTlm/telemetry_definitions.c
  src/src_user/CmdTlm/user_packet_handler.c
  src/src_user/CmdTlm/NormalBlockCommandDefinition/nbc_start_hk_tlm.c
  src/src_user/CmdTlm/NormalBlockCommandDefinition/nbc_respond_eh_for_test.c
  src/src_user/CmdTlm/Ccsds/m_pdu.c
  src/src_user/CmdTlm/Ccsds/TCFrame.c
  src/src_user/CmdTlm/Ccsds/tcp_to_m_pdu.c
  src/src_user/CmdTlm/Ccsds/TCPacket.c
  src/src_user/CmdTlm/Ccsds/TCSegment.c
  src/src_user/CmdTlm/Ccsds/vcdu.c
  src/src_user/Drivers/Com/gs.c
  src/src_user/Drivers/Com/gs_validate.c
  src/src_user/Drivers/Etc/UART_TEST.c
  src/src_user/IfWrapper/if_list.c
  src/src_user/Library/git_revision.c
  src/src_user/Library/print.c
  src/src_user/Library/VT100.c
  src/src_user/Settings/Applications/AnomalyHandlerRules/ahr_default_load.c
  src/src_user/Settings/AnomalyLogger/al_default_settings.c
  src/src_user/Settings/Modes/mode_definitions.c
  src/src_user/Settings/Modes/TaskLists/InitialTL.c
  src/src_user/Settings/Modes/TaskLists/Composition/debug_display.c
  src/src_user/Settings/Modes/TaskLists/Composition/drivers_update.c
  src/src_user/Settings/Modes/TaskLists/Composition/gs_related_process.c
  src/src_user/Settings/Modes/TaskLists/Composition/tlm_cmd_hirate.c
  src/src_user/Settings/Modes/Transitions/InitialSL.c
  src/src_user/Settings/Modes/Transitions/NopSL.c
  src/src_user/Settings/Modes/Transitions/SequenceItems.c
  src/src_user/Settings/System/event_logger_settings.c
  src/src_user/Settings/System/EventHandlerRules/event_handler_rules.c
  src/src_user/Settings/System/EventHandlerRules/event_handler_rule_test.c
)
target_link_libraries(${PROJECT_NAME} C2A_CORE)

# 通常，S2EではC++ビルドされるが，C2A core開発のため，C2A単体をC89でライブラリビルドする場合は，mockupをビルド対象にする
if(USE_SILS_MOCKUP)
  target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/SilsMockup/ccsds_sils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/SilsMockup/uart_sils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/SilsMockup/wdt_sils.c
  )
  message("USE SILS_MOCKUP")
else()
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/Sils/ccsds_sils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/Sils/uart_sils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/Sils/wdt_sils.cpp
  )
endif()

# SCI COM for connection to WINGS TMTC IF
# ！！！注意！！！
# これをONにした状態で，SCIの受け口がない場合（TMTC IFが動いてない状態）
# そちらのバッファが詰まってSILSの動作が止まることがあるので注意すること！
option(USE_SCI_COM_WINGS "Use SCI_COM_WINGS")
set(USE_SCI_COM_WINGS ON)
if(USE_SCI_COM_WINGS AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_WINGS)
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/src_user/IfWrapper/Sils/ccsds_sils_sci_if.cpp
  )
  message("USE SCI_COM_WINGS")
endif()

# SCI COM for connection to PC UART
# ！！！注意！！！
# これをONにした状態で，SCIの受け口がない場合（受けてのTeratermが起動していない状態）
# そちらのバッファが詰まってSILSの動作が止まることがあるので注意すること！
option(USE_SCI_COM_UART "Use SCI_COM_UART")
set(USE_SCI_COM_UART OFF)
if(USE_SCI_COM_UART AND NOT USE_SILS_MOCKUP)
  add_definitions(-DUSE_SCI_COM_UART)
  target_sources(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}src/src_user/IfWrapper/Sils/uart_sils_sci_if.cpp
  )
  message("USE SCI_COM_UART")
endif()

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
