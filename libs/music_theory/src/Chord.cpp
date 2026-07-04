#include "maestro/theory/Chord.h"

namespace maestro::theory
{
    namespace
    {
        std::array<int, 3> triadIntervals(ChordQuality quality)
        {
            switch (quality)
            {
                case ChordQuality::Major:      return { 0, 4, 7 };
                case ChordQuality::Minor:      return { 0, 3, 7 };
                case ChordQuality::Diminished: return { 0, 3, 6 };
            }
            return { 0, 4, 7 };
        }
    }

    std::array<int, 3> triadMidiNotes(const Chord& chord, int octave)
    {
        const int rootMidiNote = (octave + 1) * 12 + static_cast<int>(chord.root);
        const auto intervals = triadIntervals(chord.quality);
        return { rootMidiNote + intervals[0], rootMidiNote + intervals[1], rootMidiNote + intervals[2] };
    }

    std::string toString(const Chord& chord)
    {
        std::string suffix;
        switch (chord.quality)
        {
            case ChordQuality::Major:      suffix = "";    break;
            case ChordQuality::Minor:      suffix = "m";   break;
            case ChordQuality::Diminished: suffix = "dim"; break;
        }
        return toString(chord.root) + suffix;
    }
}
