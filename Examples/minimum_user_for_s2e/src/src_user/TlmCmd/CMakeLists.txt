cmake_minimum_required(VERSION 3.13)

project(C2A_USER_CMD_TLM)

set(C2A_SRCS
  block_command_definitions.c
  block_command_user_settings.c
  command_definitions.c
  common_cmd_packet.c
  common_tlm_cmd_packet.c
  common_tlm_packet.c
  telemetry_definitions.c
  user_packet_handler.c
  NormalBlockCommandDefinition/nbc_start_hk_tlm.c
  NormalBlockCommandDefinition/nbc_respond_eh_for_test.c
  Ccsds/m_pdu.c
  Ccsds/TCFrame.c
  Ccsds/tcp_to_m_pdu.c
  Ccsds/TCSegment.c
  Ccsds/vcdu.c
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
