cmake_minimum_required(VERSION 3.5)
project(spell_corrector)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES spell-correct.cpp)
add_executable(spell_corrector ${SOURCE_FILES})