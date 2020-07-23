cmake_minimum_required(VERSION 3.10)

project(HackerTyper VERSION 1.0)

if (WIN32)
else()
set(SHARED_FLAGS " -Wall -Wextra -Wpedantic -Wshadow -Werror")
set(CMAKE_C_FLAGS "-std=c11 ${SHARED_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++11 ${SHARED_FLAGS}")
endif()


configure_file(config.h.in config.h)

add_executable(HackerTyper hackertyper.cpp)

if (WIN32)
else()
target_link_libraries(HackerTyper ncurses)
endif()
