cmake_minimum_required(VERSION 3.2)
project(datastructure_performance_test)

find_package(Boost 1.58 EXACT
    REQUIRED COMPONENTS program_options random)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")

set(SOURCE_FILES main.cpp)
add_executable(datastructure_performance_test ${SOURCE_FILES} test_data_generator.cpp test_data_generator.h)
target_link_libraries(datastructure_performance_test ${Boost_LIBRARIES})
