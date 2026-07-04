#include "maestro/theory/Chord.h"
#include <set>

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

    std::optional<Chord> recognizeChord(const std::vector<int>& midiNotes)
    {
        std::set<int> pitchClasses;
        for (int note : midiNotes)
            pitchClasses.insert(((note % 12) + 12) % 12);

        if (pitchClasses.size() != 3)
            return std::nullopt;

        for (int root = 0; root < 12; ++root)
        {
            for (ChordQuality quality : { ChordQuality::Major, ChordQuality::Minor, ChordQuality::Diminished })
            {
                std::set<int> expected;
                for (int interval : triadIntervals(quality))
                    expected.insert((root + interval) % 12);

                if (expected == pitchClasses)
                    return Chord { static_cast<PitchClass>(root), quality };
            }
        }
        return std::nullopt;
    }
}
