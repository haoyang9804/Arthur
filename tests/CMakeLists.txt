file(
  GLOB_RECURSE TESTS
  *.cpp
)

add_executable(
  googletest_${PROJECT_NAME}
  ${TESTS}
)

target_include_directories(
  googletest_${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  googletest_${PROJECT_NAME}
  PRIVATE
  ${PROJECT_NAME}_lib
  GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(googletest_${PROJECT_NAME})