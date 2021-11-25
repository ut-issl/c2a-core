cmake_minimum_required(VERSION 3.10)

project(C2A_USER_LIB)

add_library(${PROJECT_NAME} STATIC
  git_revision.c
  print.c
  VT100.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
