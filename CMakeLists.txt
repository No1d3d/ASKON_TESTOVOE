cmake_minimum_required(VERSION 3.20)
project(auth_app LANGUAGES CXX)

# Стандарт
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# От ошибок Conan
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

# Зависимости
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)

# Библиотека с логикой
add_library(auth_lib
    src/auth.cpp
)

target_include_directories(auth_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(auth_lib
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Основное приложение
add_executable(auth_app
    src/main.cpp
)

target_link_libraries(auth_app
    auth_lib
)

# Тесты
enable_testing()

add_executable(auth_tests
    tests/test_auth.cpp
)

target_link_libraries(auth_tests
    auth_lib
    GTest::gtest_main
)

add_test(NAME auth_tests COMMAND auth_tests)