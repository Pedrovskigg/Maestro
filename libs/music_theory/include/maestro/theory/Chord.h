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
        Diminished
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

    // Identifies a plain triad (in any inversion/octave) from a set of currently-held MIDI notes.
    // Returns nullopt if the notes don't form exactly one of the triads this engine models.
    std::optional<Chord> recognizeChord(const std::vector<int>& midiNotes);
}
