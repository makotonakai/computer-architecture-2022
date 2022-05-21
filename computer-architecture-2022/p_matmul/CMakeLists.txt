cmake_minimum_required(VERSION 3.10.2)
project(p_matmul)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP)
add_executable(p_matmul main.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(p_matmul PUBLIC OpenMP::OpenMP_CXX)
endif()