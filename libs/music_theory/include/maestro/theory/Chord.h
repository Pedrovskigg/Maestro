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

    // Extra colour on top of the plain triad. Seventh needs extensionInterval set to the correct
    // diatonic 7th (10 or 11 semitones above the root) - DiatonicChordGenerator computes that per
    // scale degree, since e.g. a Major-quality I is a maj7 but a Major-quality V is a dominant 7.
    enum class ChordExtension
    {
        Triad,
        Seventh,
        Add9,
        PowerFifth // "5" - root + fifth only, no third
    };

    struct Chord
    {
        PitchClass root;
        ChordQuality quality;
        ChordExtension extension = ChordExtension::Triad;
        int extensionInterval = 0; // semitones above root for the 7th (10 or 11); unused otherwise

        bool operator==(const Chord& other) const
        {
            // Extension is deliberately excluded: a G7 is still functionally "the V chord" as far
            // as key-relative lookups (diatonic degree, suggestions) are concerned.
            return root == other.root && quality == other.quality;
        }
    };

    // MIDI note numbers for the chord's plain triad in the given octave (e.g. octave 4 -> starts
    // near middle C). Always exactly 3 notes, regardless of chord.extension.
    std::array<int, 3> triadMidiNotes(const Chord& chord, int octave);

    // MIDI note numbers honouring chord.extension (2 notes for a power chord, 4 for a 7th/add9).
    std::vector<int> chordMidiNotes(const Chord& chord, int octave);

    std::string toString(const Chord& chord);

    // Identifies a plain triad (in any inversion/octave, root/3rd/5th - extra notes such as a 7th or
    // doubled tones are tolerated) from a set of currently-held MIDI notes. Returns nullopt if no
    // triad this engine models is found within the held notes.
    std::optional<Chord> recognizeChord(const std::vector<int>& midiNotes);
}
