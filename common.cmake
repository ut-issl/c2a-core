if(BUILD_C2A_AS_CXX)
  # memo: set_source_files_properties() must be set before add_library/add_executable on Visual Studio CMake
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
  set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX) # C++
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
endif()

# Compile option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  #target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragma")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-finput-charset=cp932 -m32 -rdynamic -Wall -g -Wno-unknown-pragmas") # SJIS, 32bit
endif()
