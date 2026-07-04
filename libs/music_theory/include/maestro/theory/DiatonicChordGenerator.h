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
    };

    std::array<DiatonicChord, 7> generateDiatonicChords(const Key& key);
}
