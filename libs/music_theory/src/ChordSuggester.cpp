#include "maestro/theory/ChordSuggester.h"
#include "maestro/theory/DiatonicChordGenerator.h"
#include <algorithm>

namespace maestro::theory
{
    namespace
    {
        struct Transition
        {
            int toDegree;
            double weight;
            const char* rationale;
        };

        const std::vector<std::vector<Transition>> kMajorTransitions {
            /* I    */ { { 4, 0.9, "Tonica -> Subdominante" }, { 5, 0.9, "Tonica -> Dominante" },
                         { 6, 0.6, "Tonica -> relativa (vi)" }, { 2, 0.4, "Tonica -> ii" } },
            /* ii   */ { { 5, 0.9, "Subdominante -> Dominante" }, { 4, 0.3, "ii -> IV" }, { 1, 0.2, "ii -> Tonica" } },
            /* iii  */ { { 6, 0.7, "iii -> vi (relativa menor)" }, { 4, 0.5, "iii -> IV" }, { 1, 0.3, "iii -> Tonica" } },
            /* IV   */ { { 5, 0.9, "Subdominante -> Dominante" }, { 1, 0.7, "Subdominante -> Tonica" }, { 2, 0.3, "IV -> ii" } },
            /* V    */ { { 1, 1.0, "Dominante -> Tonica (resolucao classica)" }, { 6, 0.5, "Cadencia de engano (V -> vi)" },
                         { 4, 0.2, "V -> IV" } },
            /* vi   */ { { 4, 0.8, "vi -> IV (progressao pop comum)" }, { 5, 0.6, "vi -> V" }, { 2, 0.4, "vi -> ii" },
                         { 1, 0.3, "vi -> Tonica" } },
            /* vii deg */ { { 1, 0.95, "Dominante -> Tonica (resolucao)" } },
        };

        const std::vector<std::vector<Transition>> kNaturalMinorTransitions {
            /* i    */ { { 4, 0.8, "Tonica -> Subdominante" }, { 6, 0.7, "Tonica -> VI" },
                         { 7, 0.6, "Tonica -> VII" }, { 5, 0.4, "Tonica -> Dominante" } },
            /* ii°  */ { { 5, 0.7, "Subdominante -> Dominante" }, { 1, 0.3, "ii deg -> Tonica" } },
            /* III  */ { { 6, 0.6, "III -> VI" }, { 4, 0.4, "III -> iv" }, { 1, 0.3, "III -> Tonica" } },
            /* iv   */ { { 1, 0.7, "Subdominante -> Tonica" }, { 5, 0.6, "Subdominante -> Dominante" }, { 7, 0.4, "iv -> VII" } },
            /* v    */ { { 1, 0.8, "Dominante -> Tonica" }, { 6, 0.4, "v -> VI" } },
            /* VI   */ { { 7, 0.7, "VI -> VII" }, { 1, 0.5, "VI -> Tonica" }, { 4, 0.4, "VI -> iv" } },
            /* VII  */ { { 1, 0.85, "VII -> Tonica (resolucao comum em rock)" } },
        };
    }

    std::vector<ChordSuggestion> suggestNextChords(const Key& key,
                                                     const std::vector<Chord>& progressionSoFar,
                                                     int maxResults)
    {
        const auto diatonic = generateDiatonicChords(key);
        const auto degreeInfo = [&](int degree) { return diatonic[static_cast<size_t>(degree - 1)]; };

        std::vector<ChordSuggestion> results;

        int lastDegree = 0;
        if (!progressionSoFar.empty())
        {
            const Chord& last = progressionSoFar.back();
            for (const auto& d : diatonic)
                if (d.chord == last) { lastDegree = d.degree; break; }
        }

        if (lastDegree == 0)
        {
            // No progression yet, or the last chord isn't diatonic to this key: start from tonic-function chords.
            for (const auto& d : diatonic)
                if (d.function == HarmonicFunction::Tonic)
                    results.push_back({ d.degree, d.chord, d.degree == 1 ? 1.0 : 0.5, "Ponto de partida (funcao tonica)" });
        }
        else
        {
            const auto& transitions = key.type == ScaleType::Major
                                           ? kMajorTransitions[static_cast<size_t>(lastDegree - 1)]
                                           : kNaturalMinorTransitions[static_cast<size_t>(lastDegree - 1)];
            for (const auto& t : transitions)
                results.push_back({ t.toDegree, degreeInfo(t.toDegree).chord, t.weight, t.rationale });
        }

        std::sort(results.begin(), results.end(),
                  [](const ChordSuggestion& a, const ChordSuggestion& b) { return a.weight > b.weight; });

        if (static_cast<int>(results.size()) > maxResults)
            results.resize(static_cast<size_t>(maxResults));

        return results;
    }
}
