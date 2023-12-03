cmake_minimum_required(VERSION 3.16)
project(Speedoku LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Speedoku src/main.cpp src/SudokuBoard.cpp src/SudokuCell.cpp
        src/AlgorithmManager.cpp)
target_link_libraries(Speedoku PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
target_compile_features(Speedoku PRIVATE cxx_std_14)