cmake_minimum_required(VERSION 3.25)
project(simopticon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(simopticon src/evaluation/Evaluation.h src/main.cpp src/evaluation/constant_headway/ConstantHeadway.h src/evaluation/constant_headway/ConstantHeadway.cpp src/utils/CommandLine.cpp src/utils/CommandLine.h src/parameters/ParameterDefinition.h src/parameters/Parameter.h src/parameters/ParameterDefinition.cpp src/parameters/Parameter.cpp src/parameters/ContinuousParameter.cpp src/parameters/DiscreteParameter.h src/parameters/DiscreteParameter.cpp src/controller/Controller.h src/optimizer/direct/hyrect/HyRect.cpp src/optimizer/direct/hyrect/HyRect.h src/optimizer/direct/DirectOptimizer.cpp src/optimizer/direct/DirectOptimizer.h src/optimizer/direct/StoppingCondition.cpp src/optimizer/direct/StoppingCondition.h src/optimizer/direct/Levels.cpp src/optimizer/direct/Levels.h src/optimizer/Optimizer.cpp src/optimizer/Optimizer.h src/controller/Controller.cpp src/Types.h src/optimizer/direct/ParameterNormalizer.cpp src/optimizer/direct/ParameterNormalizer.h src/optimizer/direct/DirectTypes.h src/optimizer/direct/GrahamScan.cpp src/optimizer/direct/GrahamScan.h src/controller/ValueMap.cpp src/controller/ValueMap.h src/optimizer/direct/hyrect/BaseRect.cpp src/optimizer/direct/hyrect/BaseRect.h src/optimizer/direct/hyrect/ChildRect.cpp src/optimizer/direct/hyrect/ChildRect.h src/controller/StubController.cpp src/controller/StubController.h src/runner/plexe/PlexeSimulationRunner.cpp src/runner/plexe/PlexeSimulationRunner.h src/ComparisonFunctions.h src/optimizer/direct/DirectComparisonFunctions.h src/runner/SimulationRunner.cpp src/runner/plexe/ConfigEditor.cpp src/runner/plexe/ConfigEditor.h src/utils/PythonScript.cpp src/utils/PythonScript.h src/utils/Multithreaded.h src/utils/Multithreaded.tpp src/evaluation/Evaluation.cpp src/utils/ThreadsafeQueue.tpp src/utils/ThreadsafeQueue.h src/status/Status.h src/status/Status.cpp src/status/StatusBar.cpp src/status/StatusBar.h src/utils/Abortable.cpp src/utils/Abortable.h)

target_link_libraries(simopticon PRIVATE ${Python3_LIBRARIES} nlohmann_json::nlohmann_json pthread stdc++fs)