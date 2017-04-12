cmake_minimum_required(VERSION 3.7)
project(sample_code)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cc
        main.hh
        moc_main.cpp)

add_executable(p2papp ${SOURCE_FILES})