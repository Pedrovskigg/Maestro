add_library(maestro_theory STATIC
    src/Version.cpp
)

target_include_directories(maestro_theory PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(maestro_theory PUBLIC cxx_std_17)

if (MAESTRO_BUILD_TESTS)
    add_subdirectory(tests)
endif()
