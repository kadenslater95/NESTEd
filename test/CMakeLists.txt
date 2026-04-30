include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip
  DOWNLOAD_EXTRACT_TIMESTAMP GENERATE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Uncomment for debugging purposes
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")


enable_testing()


add_executable(
  nested_tests
  file_utils_tests/read_file_tests.cpp
)


target_link_libraries(nested_tests GTest::gtest_main file_utils)


include(GoogleTest)

gtest_discover_tests(nested_tests)