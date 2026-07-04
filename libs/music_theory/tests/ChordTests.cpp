#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/Chord.h"

using namespace maestro::theory;

TEST_CASE("triadMidiNotes returns the correct notes for C major octave 4", "[chord]")
{
    Chord chord { PitchClass::C, ChordQuality::Major };
    const auto notes = triadMidiNotes(chord, 4);

    REQUIRE(notes[0] == 60); // C4
    REQUIRE(notes[1] == 64); // E4
    REQUIRE(notes[2] == 67); // G4
}

TEST_CASE("recognizeChord identifies a root-position triad", "[chord]")
{
    const auto result = recognizeChord({ 60, 64, 67 }); // C E G

    REQUIRE(result.has_value());
    REQUIRE(result->root == PitchClass::C);
    REQUIRE(result->quality == ChordQuality::Major);
}

TEST_CASE("recognizeChord identifies the same chord regardless of inversion or octave", "[chord]")
{
    const auto result = recognizeChord({ 64, 67, 72 }); // E G C (first inversion, C up an octave)

    REQUIRE(result.has_value());
    REQUIRE(result->root == PitchClass::C);
    REQUIRE(result->quality == ChordQuality::Major);
}

TEST_CASE("recognizeChord identifies a diminished triad", "[chord]")
{
    const auto result = recognizeChord({ 71, 74, 77 }); // B D F

    REQUIRE(result.has_value());
    REQUIRE(result->root == PitchClass::B);
    REQUIRE(result->quality == ChordQuality::Diminished);
}

TEST_CASE("recognizeChord returns nullopt for notes that don't form a modeled triad", "[chord]")
{
    REQUIRE_FALSE(recognizeChord({ 60, 61, 62 }).has_value());
    REQUIRE_FALSE(recognizeChord({ 60, 64 }).has_value());
    REQUIRE_FALSE(recognizeChord({}).has_value());
}
