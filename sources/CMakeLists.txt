cmake_minimum_required(VERSION 3.14)
project(sources)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(PkgConfig REQUIRED)
find_package (Threads REQUIRED)
PKG_CHECK_MODULES(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(sources
        board.cpp
        board.h
        graphics.cpp
        graphics.h
        main.cpp
        worm.cpp
        worm.h)

target_compile_options(sources PRIVATE -Wall -Wextra -Werror)
target_link_libraries(sources ${GTKMM_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
