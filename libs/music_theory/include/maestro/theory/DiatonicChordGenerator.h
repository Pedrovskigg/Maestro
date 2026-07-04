#pragma once

#include <array>
#include <string>
#include "maestro/theory/Chord.h"
#include "maestro/theory/Key.h"

namespace maestro::theory
{
    enum class HarmonicFunction
    {
        Tonic,
        Subdominant,
        Dominant
    };

    struct DiatonicChord
    {
        int degree; // 1-7
        Chord chord;
        std::string romanNumeral;
        HarmonicFunction function;
        int seventhInterval; // semitones from this chord's root to its diatonic 7th (10 or 11)
    };

    // Chord.h's chordMidiNotes() needs extensionInterval set correctly for ChordExtension::Seventh -
    // this returns a copy of the diatonic chord's triad wired up with the right 7th for its degree.
    Chord withSeventh(const DiatonicChord& diatonic);

    std::array<DiatonicChord, 7> generateDiatonicChords(const Key& key);

    // Short, beginner-friendly explanation of what a harmonic function does, for teaching UI (tooltips, panels, etc.).
    std::string describe(HarmonicFunction function);
}
