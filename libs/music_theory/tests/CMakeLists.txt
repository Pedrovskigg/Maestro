include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(maestro_theory_tests
    VersionTests.cpp
    PitchClassTests.cpp
    KeyTests.cpp
    DiatonicChordGeneratorTests.cpp
    ChordSuggesterTests.cpp
)

target_link_libraries(maestro_theory_tests PRIVATE
    maestro_theory
    Catch2::Catch2WithMain
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(maestro_theory_tests)
