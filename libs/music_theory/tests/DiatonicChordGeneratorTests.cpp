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

TEST_CASE("C major diatonic 7ths match textbook chords (I & IV maj7, rest m7/dominant)", "[diatonic]")
{
    Key key { PitchClass::C, ScaleType::Major };
    const auto chords = generateDiatonicChords(key);

    REQUIRE(chords[0].seventhInterval == 11); // Cmaj7
    REQUIRE(chords[1].seventhInterval == 10); // Dm7
    REQUIRE(chords[2].seventhInterval == 10); // Em7
    REQUIRE(chords[3].seventhInterval == 11); // Fmaj7
    REQUIRE(chords[4].seventhInterval == 10); // G7 (dominant)
    REQUIRE(chords[5].seventhInterval == 10); // Am7
    REQUIRE(chords[6].seventhInterval == 10); // Bm7b5 (half-diminished)

    const auto v7 = withSeventh(chords[4]);
    REQUIRE(toString(v7) == "G7");

    const auto imaj7 = withSeventh(chords[0]);
    REQUIRE(toString(imaj7) == "Cmaj7");

    const auto viihalfdim = withSeventh(chords[6]);
    REQUIRE(toString(viihalfdim) == "Bm7b5");
}

TEST_CASE("A natural minor diatonic 7ths match textbook chords (III & VI maj7, VII dominant)", "[diatonic]")
{
    Key key { PitchClass::A, ScaleType::NaturalMinor };
    const auto chords = generateDiatonicChords(key);

    REQUIRE(chords[0].seventhInterval == 10); // Am7
    REQUIRE(chords[2].seventhInterval == 11); // Cmaj7
    REQUIRE(chords[5].seventhInterval == 11); // Fmaj7
    REQUIRE(chords[6].seventhInterval == 10); // G7 (dominant, even though VII is a major triad)

    const auto vii7 = withSeventh(chords[6]);
    REQUIRE(toString(vii7) == "G7");
}
