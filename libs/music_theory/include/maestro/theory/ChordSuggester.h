#pragma once

#include <string>
#include <vector>
#include "maestro/theory/Chord.h"
#include "maestro/theory/Key.h"

namespace maestro::theory
{
    struct ChordSuggestion
    {
        int degree;
        Chord chord;
        double weight; // 0..1, higher = more common/idiomatic
        std::string rationale;
    };

    // Suggests likely next chords given the progression built so far, using a static table of
    // common pop/rock functional-harmony transitions (no ML/statistics involved).
    std::vector<ChordSuggestion> suggestNextChords(const Key& key,
                                                     const std::vector<Chord>& progressionSoFar,
                                                     int maxResults = 4);
}
