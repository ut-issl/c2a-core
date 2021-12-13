if(BUILD_C2A_AS_CXX)
  # memo: set_source_files_properties() must be set before add_library/add_executable on Visual Studio CMake
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
  set_target_properties(${PROJECT_NAME} PROPERTIES LANGUAGE CXX) # C++
else()
  if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # TODO: remove this!
    # -Wno-commentが`std=c90`の後に来る必要があるのでC89のうちはこうするしかない
    target_compile_options(${PROJECT_NAME} PUBLIC "-std=c90")
  else()
    set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 90) # C89
    set_target_properties(${PROJECT_NAME} PROPERTIES C_EXTENSIONS FALSE) # no extensions(GNU)
  endif()
endif()

# Build option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
else()
  # SJIS
  if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PUBLIC "-finput-charset=cp932")
  endif()

  # 32bit
  target_compile_options(${PROJECT_NAME} PUBLIC "-m32")
  target_link_options(${PROJECT_NAME} PRIVATE "-m32")

  # debug
  target_compile_options(${PROJECT_NAME} PUBLIC "-g")
  if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PUBLIC "-rdynamic")
  endif()

  # warning
  target_compile_options(${PROJECT_NAME} PUBLIC "-Wpedantic")
  target_compile_options(${PROJECT_NAME} PUBLIC "-Wall")
  if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # gccの-Wcommentは2重コメントにしか影響しない
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-comment")
  endif()
  target_compile_options(${PROJECT_NAME} PUBLIC "-Wno-unknown-pragmas")
  if(ADD_WERROR_FLAGS)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Werror")
  endif()
  if(ADD_WEXTRA_FLAGS)
    target_compile_options(${PROJECT_NAME} PUBLIC "-Wextra")
  endif()
endif()
