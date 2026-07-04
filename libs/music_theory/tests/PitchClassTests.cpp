#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/PitchClass.h"

using namespace maestro::theory;

TEST_CASE("transpose wraps around the octave", "[pitchclass]")
{
    REQUIRE(transpose(PitchClass::C, 0) == PitchClass::C);
    REQUIRE(transpose(PitchClass::C, 11) == PitchClass::B);
    REQUIRE(transpose(PitchClass::C, 12) == PitchClass::C);
    REQUIRE(transpose(PitchClass::A, 4) == PitchClass::CSharp);
}

TEST_CASE("transpose handles negative semitones", "[pitchclass]")
{
    REQUIRE(transpose(PitchClass::C, -1) == PitchClass::B);
    REQUIRE(transpose(PitchClass::D, -2) == PitchClass::C);
}

TEST_CASE("toString names match standard sharp spelling", "[pitchclass]")
{
    REQUIRE(toString(PitchClass::C) == "C");
    REQUIRE(toString(PitchClass::FSharp) == "F#");
    REQUIRE(toString(PitchClass::B) == "B");
}
