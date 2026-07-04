#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "maestro/theory/PitchClass.h"

namespace maestro::theory
{
    enum class ChordQuality
    {
        Major,
        Minor,
        Diminished,
        Augmented
    };

    struct Chord
    {
        PitchClass root;
        ChordQuality quality;

        bool operator==(const Chord& other) const
        {
            return root == other.root && quality == other.quality;
        }
    };

    // MIDI note numbers for the chord's triad in the given octave (e.g. octave 4 -> starts near middle C).
    std::array<int, 3> triadMidiNotes(const Chord& chord, int octave);

    std::string toString(const Chord& chord);

    // Identifies a plain triad (in any inversion/octave, root/3rd/5th - extra notes such as a 7th or
    // doubled tones are tolerated) from a set of currently-held MIDI notes. Returns nullopt if no
    // triad this engine models is found within the held notes.
    std::optional<Chord> recognizeChord(const std::vector<int>& midiNotes);
}
