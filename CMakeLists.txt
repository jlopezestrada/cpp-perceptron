cmake_minimum_required(VERSION 3.16)

project(cpp_perceptron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(enable_project_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    endif()
endfunction()

add_library(perceptron src/Perceptron.cpp src/DecisionBoundaryVisualizer.cpp)
target_include_directories(perceptron PUBLIC include)
enable_project_warnings(perceptron)

add_executable(cpp-perceptron src/main.cpp)
target_link_libraries(cpp-perceptron PRIVATE perceptron)
enable_project_warnings(cpp-perceptron)

enable_testing()

add_executable(perceptron_tests tests/PerceptronTests.cpp)
target_link_libraries(perceptron_tests PRIVATE perceptron)
enable_project_warnings(perceptron_tests)

add_test(NAME perceptron_tests COMMAND perceptron_tests)
