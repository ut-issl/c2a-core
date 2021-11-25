cmake_minimum_required(VERSION 3.10)

project(C2A_USER_CMD_TLM)

add_library(${PROJECT_NAME} STATIC
  block_command_definitions.c
  block_command_user_settings.c
  command_definitions.c
  common_tlm_cmd_packet.c
  telemetry_definitions.c
  user_packet_handler.c
  NormalBlockCommandDefinition/nbc_start_hk_tlm.c
  Ccsds/m_pdu.c
  Ccsds/TCFrame.c
  Ccsds/tcp_to_m_pdu.c
  Ccsds/TCPacket.c
  Ccsds/TCSegment.c
  Ccsds/vcdu.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
