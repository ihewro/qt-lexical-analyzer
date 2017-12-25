cmake_minimum_required(VERSION 3.8)
project(lexcode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp mygraph.cpp mygraph.h lex.cpp lex.h)
add_executable(lexcode ${SOURCE_FILES})