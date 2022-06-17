cmake_minimum_required(VERSION 3.13)

project(C2A_USER_SETTINGS)

set(C2A_SRCS
  Applications/AnomalyHandlerRules/ahr_default_load.c
  Modes/mode_definitions.c
  Modes/TaskLists/tl_initial.c
  Modes/TaskLists/Elements/tl_elem_debug_display.c
  Modes/TaskLists/Elements/tl_elem_drivers_update.c
  Modes/TaskLists/Elements/tl_elem_gs_related_process.c
  Modes/TaskLists/Elements/tl_elem_tlm_cmd_hirate.c
  Modes/Transitions/sl_initial.c
  Modes/Transitions/sl_nop.c
  Modes/Transitions/sequence_items.c
  System/anomaly_logger_settings.c
  System/event_logger_settings.c
  System/EventHandlerRules/event_handler_rules.c
  System/EventHandlerRules/event_handler_rule_test.c
  TlmCmd/common_cmd_packet_define.c
  TlmCmd/Ccsds/apid_define.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
