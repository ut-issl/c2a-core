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
    # TODO: set always!
    # GNU拡張を禁止すると1行コメントがエラーになる
    if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set_target_properties(${PROJECT_NAME} PROPERTIES C_EXTENSIONS FALSE) # no extensions(GNU)
    endif()
  endif()
endif()

if(BUILD_C2A_AS_SILS_FW)
  target_compile_definitions(${PROJECT_NAME} PUBLIC SILS_FW)
endif()

# Build option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/MT")
  if(BUILD_C2A_AS_CXX)
    target_compile_options(${PROJECT_NAME} PUBLIC "/TP") # Compile C codes as C++
  endif()
  if(BUILD_C2A_AS_UTF8)
    target_compile_options(${PROJECT_NAME} PUBLIC "/source-charset:utf-8")
  endif()
else()
  # SJIS
  # if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
  #   target_compile_options(${PROJECT_NAME} PUBLIC "-finput-charset=cp932")
  # endif()

  # 32bit
  if(NOT USE_32BIT_COMPILER)
    target_compile_options(${PROJECT_NAME} PUBLIC "-m32")
    target_link_options(${PROJECT_NAME} PRIVATE "-m32")
  endif()

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
