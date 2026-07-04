cmake_minimum_required(VERSION 3.22)
project(Maestro VERSION 0.1.0 LANGUAGES CXX)

option(MAESTRO_BUILD_TESTS "Build music_theory unit tests" ON)

if (MAESTRO_BUILD_TESTS)
    enable_testing()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/JUCE)
add_subdirectory(libs/music_theory)
add_subdirectory(plugin)
