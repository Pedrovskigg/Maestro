#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/Key.h"

using namespace maestro::theory;

TEST_CASE("C major degrees are the natural notes", "[key]")
{
    Key key { PitchClass::C, ScaleType::Major };
    const auto degrees = key.degreePitchClasses();

    REQUIRE(degrees[0] == PitchClass::C);
    REQUIRE(degrees[1] == PitchClass::D);
    REQUIRE(degrees[2] == PitchClass::E);
    REQUIRE(degrees[3] == PitchClass::F);
    REQUIRE(degrees[4] == PitchClass::G);
    REQUIRE(degrees[5] == PitchClass::A);
    REQUIRE(degrees[6] == PitchClass::B);
}

TEST_CASE("A natural minor degrees match its relative major notes", "[key]")
{
    Key key { PitchClass::A, ScaleType::NaturalMinor };
    const auto degrees = key.degreePitchClasses();

    REQUIRE(degrees[0] == PitchClass::A);
    REQUIRE(degrees[1] == PitchClass::B);
    REQUIRE(degrees[2] == PitchClass::C);
    REQUIRE(degrees[3] == PitchClass::D);
    REQUIRE(degrees[4] == PitchClass::E);
    REQUIRE(degrees[5] == PitchClass::F);
    REQUIRE(degrees[6] == PitchClass::G);
}

TEST_CASE("degree pitch classes transpose correctly for a non-C root", "[key]")
{
    Key key { PitchClass::D, ScaleType::Major };
    const auto degrees = key.degreePitchClasses();

    REQUIRE(degrees[0] == PitchClass::D);
    REQUIRE(degrees[3] == PitchClass::G);
    REQUIRE(degrees[4] == PitchClass::A);
}
