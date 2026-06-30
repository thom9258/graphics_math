cmake_minimum_required(VERSION 3.1)
project(benchmark)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2 -msse2")

add_executable(${PROJECT_NAME} main.cpp)

find_package(benchmark REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC benchmark::benchmark glm::glm)
