#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/DiatonicChordGenerator.h"

using namespace maestro::theory;

TEST_CASE("C major diatonic chords match textbook qualities", "[diatonic]")
{
    Key key { PitchClass::C, ScaleType::Major };
    const auto chords = generateDiatonicChords(key);

    REQUIRE(chords[0].chord.quality == ChordQuality::Major);      // I
    REQUIRE(chords[1].chord.quality == ChordQuality::Minor);      // ii
    REQUIRE(chords[2].chord.quality == ChordQuality::Minor);      // iii
    REQUIRE(chords[3].chord.quality == ChordQuality::Major);      // IV
    REQUIRE(chords[4].chord.quality == ChordQuality::Major);      // V
    REQUIRE(chords[5].chord.quality == ChordQuality::Minor);      // vi
    REQUIRE(chords[6].chord.quality == ChordQuality::Diminished); // vii

    REQUIRE(chords[0].romanNumeral == "I");
    REQUIRE(chords[4].romanNumeral == "V");
    REQUIRE(chords[4].function == HarmonicFunction::Dominant);
    REQUIRE(chords[0].function == HarmonicFunction::Tonic);
}

TEST_CASE("A natural minor diatonic chords match textbook qualities", "[diatonic]")
{
    Key key { PitchClass::A, ScaleType::NaturalMinor };
    const auto chords = generateDiatonicChords(key);

    REQUIRE(chords[0].chord.quality == ChordQuality::Minor);      // i
    REQUIRE(chords[1].chord.quality == ChordQuality::Diminished); // ii
    REQUIRE(chords[2].chord.quality == ChordQuality::Major);      // III
    REQUIRE(chords[3].chord.quality == ChordQuality::Minor);      // iv
    REQUIRE(chords[4].chord.quality == ChordQuality::Minor);      // v
    REQUIRE(chords[5].chord.quality == ChordQuality::Major);      // VI
    REQUIRE(chords[6].chord.quality == ChordQuality::Major);      // VII

    REQUIRE(chords[0].chord.root == PitchClass::A);
    REQUIRE(chords[5].chord.root == PitchClass::F);
}
