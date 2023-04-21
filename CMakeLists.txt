cmake_minimum_required(VERSION 3.25)
project(simopticon)

set(CMAKE_CXX_STANDARD 23)

add_executable(simopticon src/evaluation/PerformanceFunction.h src/controller/SimulationController.cpp src/controller/SimulationController.h src/optimizer/Optimizer.cpp src/optimizer/Optimizer.h src/utils/Parameter.cpp src/utils/Parameter.h src/main.cpp src/utils/DiscreteParameter.cpp src/utils/DiscreteParameter.h src/utils/ContinuousParameter.cpp src/utils/ContinuousParameter.h)
