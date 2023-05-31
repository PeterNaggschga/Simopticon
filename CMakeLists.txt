cmake_minimum_required(VERSION 3.25)
project(simopticon)

set(CMAKE_CXX_STANDARD 23)

add_executable(simopticon src/evaluation/Pipeline.h src/main.cpp src/evaluation/ConstantHeadway.h src/evaluation/ConstantHeadway.cpp src/utils/CommandLine.cpp src/utils/CommandLine.h src/parameters/ParameterDefinition.h src/parameters/Parameter.h src/parameters/ParameterDefinition.cpp src/parameters/Parameter.cpp src/parameters/ContinuousParameter.cpp src/parameters/DiscreteParameter.h src/parameters/DiscreteParameter.cpp src/Controller.h src/optimizer/direct/HyRect.cpp src/optimizer/direct/HyRect.h src/optimizer/direct/DirectOptimizer.cpp src/optimizer/direct/DirectOptimizer.h src/optimizer/direct/StoppingCondition.cpp src/optimizer/direct/StoppingCondition.h src/optimizer/direct/Levels.cpp src/optimizer/direct/Levels.h src/optimizer/Optimizer.cpp src/optimizer/Optimizer.h src/Controller.cpp src/Types.h src/optimizer/direct/ParameterNormalizer.cpp src/optimizer/direct/ParameterNormalizer.h src/optimizer/direct/DirectTypes.h src/utils/GrahamScan.cpp src/utils/GrahamScan.h)
