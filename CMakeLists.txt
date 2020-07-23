cmake_minimum_required(VERSION 3.10)

project(HackerTyper VERSION 1.0)

configure_file(config.h.in config.h)

add_executable(HackerTyper hackertyper.cpp)

target_link_libraries(HackerTyper ncurses)