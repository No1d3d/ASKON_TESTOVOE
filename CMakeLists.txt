cmake_minimum_required(VERSION 3.20)
project(auth_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies (через Conan Provider)
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)

# Library
add_library(auth_lib src/auth.cpp)

target_include_directories(auth_lib PUBLIC src)

target_link_libraries(auth_lib PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_compile_features(auth_lib PUBLIC cxx_std_17)

target_compile_options(auth_lib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# App
add_executable(auth_app src/main.cpp)
target_link_libraries(auth_app PRIVATE auth_lib)

# Tests
enable_testing()

add_executable(auth_tests tests/test_auth.cpp)

target_link_libraries(auth_tests PRIVATE
    auth_lib
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME auth_tests COMMAND auth_tests)