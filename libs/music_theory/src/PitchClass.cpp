#include "maestro/theory/PitchClass.h"
#include <array>

namespace maestro::theory
{
    PitchClass transpose(PitchClass root, int semitones)
    {
        int value = (static_cast<int>(root) + semitones) % 12;
        if (value < 0)
            value += 12;
        return static_cast<PitchClass>(value);
    }

    std::string toString(PitchClass pitchClass)
    {
        static const std::array<std::string, 12> names {
            "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
        };
        return names[static_cast<size_t>(pitchClass)];
    }
}
