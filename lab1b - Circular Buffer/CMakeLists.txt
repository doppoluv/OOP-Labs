cmake_minimum_required(VERSION 3.10.2) 
project(Lab1)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/af5ddbde4d57b0331e1d0ed3659ca2e39b37580d.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} main.cpp src/Buffer.cpp src/Buffer.h test/testLab1.cpp)

target_link_libraries(Lab1 GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(Lab1)