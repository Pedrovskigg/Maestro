#pragma once

#include <array>
#include "maestro/theory/PitchClass.h"

namespace maestro::theory
{
    enum class ScaleType
    {
        Major,
        NaturalMinor
    };

    struct Key
    {
        PitchClass root;
        ScaleType type;

        // Semitone offsets of each of the 7 scale degrees from the root.
        std::array<int, 7> degreeIntervals() const;

        // Actual pitch class of each of the 7 scale degrees.
        std::array<PitchClass, 7> degreePitchClasses() const;
    };
}
