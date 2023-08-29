cmake_minimum_required(VERSION 3.13.4)
project(Arthur)

file(
  COPY
  ${CMAKE_SOURCE_DIR}/words
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)

# this is for google test
file(
  COPY
  ${CMAKE_SOURCE_DIR}/words
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}/tests
)

file(
  GLOB_RECURSE SRCS
  src/*.cpp
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(${PROJECT_NAME}_lib SHARED ${SRCS})
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

find_program(CLANG_FORMAT_EXE NAMES "clang-format" REQUIRED)
set(CLANG_FORMAT_COMMAND "${CLANG_FORMAT_EXE}")
message(STATUS "CLANG_FORMAT_COMMAND is ${CLANG_FORMAT_COMMAND}")
file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/*.[ch]pp
)
message(STATUS "ALL_CXX_SOURCE_FILES ready for clang-format is ${ALL_CXX_SOURCE_FILES}")
add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_COMMAND}
    -i
    -style=Google
    ${ALL_CXX_SOURCE_FILES}
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(tests)