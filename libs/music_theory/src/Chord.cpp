#include "maestro/theory/Chord.h"
#include <algorithm>
#include <set>

namespace maestro::theory
{
    namespace
    {
        constexpr std::array<ChordQuality, 4> kAllQualities {
            ChordQuality::Major, ChordQuality::Minor, ChordQuality::Diminished, ChordQuality::Augmented
        };

        std::array<int, 3> triadIntervals(ChordQuality quality)
        {
            switch (quality)
            {
                case ChordQuality::Major:      return { 0, 4, 7 };
                case ChordQuality::Minor:      return { 0, 3, 7 };
                case ChordQuality::Diminished: return { 0, 3, 6 };
                case ChordQuality::Augmented:  return { 0, 4, 8 };
            }
            return { 0, 4, 7 };
        }

        std::set<int> triadPitchClasses(int root, ChordQuality quality)
        {
            std::set<int> result;
            for (int interval : triadIntervals(quality))
                result.insert((root + interval) % 12);
            return result;
        }
    }

    std::array<int, 3> triadMidiNotes(const Chord& chord, int octave)
    {
        const int rootMidiNote = (octave + 1) * 12 + static_cast<int>(chord.root);
        const auto intervals = triadIntervals(chord.quality);
        return { rootMidiNote + intervals[0], rootMidiNote + intervals[1], rootMidiNote + intervals[2] };
    }

    std::vector<int> chordMidiNotes(const Chord& chord, int octave)
    {
        const int rootMidiNote = (octave + 1) * 12 + static_cast<int>(chord.root);
        const auto triad = triadIntervals(chord.quality);

        switch (chord.extension)
        {
            case ChordExtension::PowerFifth:
                return { rootMidiNote, rootMidiNote + triad[2] };
            case ChordExtension::Add9:
                return { rootMidiNote, rootMidiNote + triad[1], rootMidiNote + triad[2], rootMidiNote + 14 };
            case ChordExtension::Seventh:
                return { rootMidiNote, rootMidiNote + triad[1], rootMidiNote + triad[2],
                         rootMidiNote + chord.extensionInterval };
            case ChordExtension::Triad:
            default:
                return { rootMidiNote, rootMidiNote + triad[1], rootMidiNote + triad[2] };
        }
    }

    std::string toString(const Chord& chord)
    {
        if (chord.extension == ChordExtension::PowerFifth)
            return toString(chord.root) + "5";

        if (chord.extension == ChordExtension::Seventh)
        {
            switch (chord.quality)
            {
                case ChordQuality::Major:
                    return toString(chord.root) + (chord.extensionInterval == 11 ? "maj7" : "7");
                case ChordQuality::Minor:
                    return toString(chord.root) + "m7";
                case ChordQuality::Diminished:
                    return toString(chord.root) + (chord.extensionInterval == 9 ? "dim7" : "m7b5");
                case ChordQuality::Augmented:
                    return toString(chord.root) + "aug7";
            }
        }

        std::string suffix;
        switch (chord.quality)
        {
            case ChordQuality::Major:      suffix = "";    break;
            case ChordQuality::Minor:      suffix = "m";   break;
            case ChordQuality::Diminished: suffix = "dim"; break;
            case ChordQuality::Augmented:  suffix = "aug"; break;
        }

        if (chord.extension == ChordExtension::Add9)
            suffix += "add9";

        return toString(chord.root) + suffix;
    }

    std::optional<Chord> recognizeChord(const std::vector<int>& midiNotes)
    {
        std::set<int> held;
        for (int note : midiNotes)
            held.insert(((note % 12) + 12) % 12);

        if (held.size() < 3)
            return std::nullopt;

        // Prefer an exact match (nothing but the triad itself) over a triad that's merely present
        // within a larger chord, so a plain C major triad isn't shadowed by, say, Cmaj7 also matching.
        for (int root = 0; root < 12; ++root)
            for (ChordQuality quality : kAllQualities)
                if (triadPitchClasses(root, quality) == held)
                    return Chord { static_cast<PitchClass>(root), quality };

        if (held.size() > 3)
        {
            for (int root = 0; root < 12; ++root)
            {
                for (ChordQuality quality : kAllQualities)
                {
                    const auto triad = triadPitchClasses(root, quality);
                    if (std::includes(held.begin(), held.end(), triad.begin(), triad.end()))
                        return Chord { static_cast<PitchClass>(root), quality };
                }
            }
        }

        return std::nullopt;
    }
}
