cmake_minimum_required(VERSION 3.14)
project(tests)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads REQUIRED)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(tests
        test.cpp ../board.cpp ../worm.cpp)

target_compile_options(tests PRIVATE -Wall -Wextra -Werror)
target_link_libraries(tests ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})