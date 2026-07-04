#include "maestro/theory/Key.h"

namespace maestro::theory
{
    namespace
    {
        constexpr std::array<int, 7> kMajorIntervals { 0, 2, 4, 5, 7, 9, 11 };
        constexpr std::array<int, 7> kNaturalMinorIntervals { 0, 2, 3, 5, 7, 8, 10 };
    }

    std::array<int, 7> Key::degreeIntervals() const
    {
        return type == ScaleType::Major ? kMajorIntervals : kNaturalMinorIntervals;
    }

    std::array<PitchClass, 7> Key::degreePitchClasses() const
    {
        const auto intervals = degreeIntervals();
        std::array<PitchClass, 7> result {};
        for (size_t i = 0; i < intervals.size(); ++i)
            result[i] = transpose(root, intervals[i]);
        return result;
    }
}
