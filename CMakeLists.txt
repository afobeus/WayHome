cmake_minimum_required(VERSION 3.11)

project(
        labwork6
        VERSION 1.0
        DESCRIPTION "Aviasales killer"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(lib)
add_subdirectory(bin)
