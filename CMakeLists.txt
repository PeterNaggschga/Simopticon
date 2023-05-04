cmake_minimum_required(VERSION 3.25)
project(simopticon)

set(CMAKE_CXX_STANDARD 23)

add_executable(simopticon src/evaluation/Pipeline.h src/main.cpp src/evaluation/ConstantHeadway.h src/evaluation/ConstantHeadway.cpp src/utils/CommandLine.cpp src/utils/CommandLine.h)
