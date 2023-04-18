cmake_minimum_required(VERSION 3.25)
project(simopticon)

set(CMAKE_CXX_STANDARD 23)

add_executable(simopticon src/evaluation/PerformanceFunction.h src/controller/SimulationController.cpp src/controller/SimulationController.h src/optimizer/Optimizer.cpp src/optimizer/Optimizer.h src/Parameter.cpp src/Parameter.h src/OmnetParameter.cpp src/OmnetParameter.h src/main.cpp)
