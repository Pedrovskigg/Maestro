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
            /* I    */ { { 4, 0.9, "Tonic to Subdominant - moves away from home to build tension" },
                         { 5, 0.9, "Tonic to Dominant - sets up a strong pull back home" },
                         { 6, 0.6, "Tonic to its relative minor (vi) - same key signature, darker color" },
                         { 2, 0.4, "Tonic to ii - a gentle step toward the Subdominant" } },
            /* ii   */ { { 5, 0.9, "Subdominant to Dominant - the classic setup for a strong resolution" },
                         { 4, 0.3, "ii to IV - both Subdominant, keeps the same function" },
                         { 1, 0.2, "ii resolving directly to the Tonic - less common, but works" } },
            /* iii  */ { { 6, 0.7, "iii to vi - both minor, a smooth relative-minor move" },
                         { 4, 0.5, "iii to IV - stepping into Subdominant territory" },
                         { 1, 0.3, "iii back to the Tonic" } },
            /* IV   */ { { 5, 0.9, "Subdominant to Dominant - builds toward a strong resolution" },
                         { 1, 0.7, "Subdominant resolving to the Tonic" },
                         { 2, 0.3, "IV to ii - both Subdominant" } },
            /* V    */ { { 1, 1.0, "Dominant to Tonic - the strongest, most natural resolution in tonal music" },
                         { 6, 0.5, "Deceptive cadence (V to vi) - expects the Tonic, lands on its relative minor instead" },
                         { 4, 0.2, "V to IV - less common, softens the pull back home" } },
            /* vi   */ { { 4, 0.8, "vi to IV - a very common move in pop and rock" },
                         { 5, 0.6, "vi to V - sets up a Dominant resolution" },
                         { 2, 0.4, "vi to ii - both minor, both Subdominant-leaning" },
                         { 1, 0.3, "vi back to the Tonic" } },
            /* vii  */ { { 1, 0.95, "Dominant-function vii\xC2\xB0 resolving up to the Tonic" } },
        };

        const std::vector<std::vector<Transition>> kNaturalMinorTransitions {
            /* i    */ { { 4, 0.8, "Tonic to Subdominant" },
                         { 6, 0.7, "Tonic to VI - very common in minor-key rock and pop" },
                         { 7, 0.6, "Tonic to VII - a favorite minor-key move" },
                         { 5, 0.4, "Tonic to Dominant (minor v - a softer pull than a major V)" } },
            /* ii°  */ { { 5, 0.7, "Subdominant to Dominant" }, { 1, 0.3, "ii\xC2\xB0 resolving to the Tonic" } },
            /* III  */ { { 6, 0.6, "III to VI" }, { 4, 0.4, "III to iv - stepping into Subdominant territory" },
                         { 1, 0.3, "III back to the Tonic" } },
            /* iv   */ { { 1, 0.7, "Subdominant resolving to the Tonic" }, { 5, 0.6, "Subdominant to Dominant" },
                         { 7, 0.4, "iv to VII" } },
            /* v    */ { { 1, 0.8, "Dominant to Tonic" }, { 6, 0.4, "v to VI" } },
            /* VI   */ { { 7, 0.7, "VI to VII - a common upward move in minor keys" },
                         { 1, 0.5, "VI back to the Tonic" }, { 4, 0.4, "VI to iv - both Subdominant-leaning" } },
            /* VII  */ { { 1, 0.85, "VII resolving to the Tonic - very common in rock" } },
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
                    results.push_back({ d.degree, d.chord, d.degree == 1 ? 1.0 : 0.5, "A good starting point - Tonic function, the key's home chord" });
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
