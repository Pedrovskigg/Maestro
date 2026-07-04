add_library(maestro_theory STATIC
    src/Version.cpp
    src/PitchClass.cpp
    src/Key.cpp
    src/Chord.cpp
    src/DiatonicChordGenerator.cpp
    src/ChordSuggester.cpp
)

target_include_directories(maestro_theory PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(maestro_theory PUBLIC cxx_std_17)

if (MAESTRO_BUILD_TESTS)
    add_subdirectory(tests)
endif()
