cmake_minimum_required(VERSION 3.5.0)
project(EIS1 VERSION 0.1.0 LANGUAGES C CXX)

set(target_name EIS1)
add_library(${target_name} INTERFACE)
target_compile_features(${target_name} INTERFACE cxx_std_20)
target_include_directories(${target_name} INTERFACE include)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    set(EIS1_ENABLE_TESTING TRUE)
endif()

if(EIS1_ENABLE_TESTING)
    add_executable(test_manual_scratch tests/manual/scratch.cpp)
    target_link_libraries(test_manual_scratch PRIVATE EIS1)
endif()
