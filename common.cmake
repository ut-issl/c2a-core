set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
  set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX) # C++
endif()
