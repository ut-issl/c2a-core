cmake_minimum_required(VERSION 3.10)

project(C2A_USER_SETTINGS)

add_library(${PROJECT_NAME} STATIC
  Applications/AnomalyHandlerRules/ahr_default_load.c
  AnomalyLogger/al_default_settings.c
  Modes/mode_definitions.c
  Modes/TaskLists/InitialTL.c
  Modes/TaskLists/Composition/debug_display.c
  Modes/TaskLists/Composition/drivers_update.c
  Modes/TaskLists/Composition/gs_related_process.c
  Modes/TaskLists/Composition/tlm_cmd_hirate.c
  Modes/Transitions/InitialSL.c
  Modes/Transitions/NopSL.c
  Modes/Transitions/SequenceItems.c
  System/event_logger_settings.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
