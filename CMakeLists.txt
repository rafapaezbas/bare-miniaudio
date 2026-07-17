cmake_minimum_required(VERSION 3.31)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-ports REQUIRED PATHS node_modules/cmake-ports)

project(bare_miniaudio C)

find_port(miniaudio)

add_bare_module(bare_miniaudio)

target_sources(
    ${bare_miniaudio}
    PRIVATE
        binding.c
)

target_link_libraries(
    ${bare_miniaudio}
    PUBLIC
        miniaudio
)
