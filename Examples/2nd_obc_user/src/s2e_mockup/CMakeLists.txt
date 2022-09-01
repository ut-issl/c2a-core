## General settings
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)

project(S2E_MOCKUP)

if(WIN32)
  add_definitions(-DWIN32)
endif()

add_definitions(-DUSE_C2A)
add_definitions(-DSILS_FW)

set(C2A_DIR ../..)
# include_directories
include_directories(${C2A_DIR}/src)
# add subdirectory
set(BUILD_C2A_AS_CXX ON)    # Build C2A as C++
add_subdirectory(${C2A_DIR} C2A)

add_executable(${PROJECT_NAME} main.cpp)

# Compile option
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
    target_compile_options(${PROJECT_NAME} PUBLIC "/source-charset:utf-8")
else()
    target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
    set(CMAKE_CXX_FLAGS "-m32 -rdynamic -Wall -g")
    set(CMAKE_C_FLAGS "-m32 -rdynamic -Wall -g")
endif()

target_link_libraries(${PROJECT_NAME} C2A)
