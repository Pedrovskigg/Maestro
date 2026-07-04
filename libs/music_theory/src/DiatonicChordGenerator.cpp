#include "maestro/theory/DiatonicChordGenerator.h"

namespace maestro::theory
{
    namespace
    {
        struct DegreeInfo
        {
            ChordQuality quality;
            const char* romanNumeral;
            HarmonicFunction function;
        };

        constexpr std::array<DegreeInfo, 7> kMajorDegrees {
            DegreeInfo { ChordQuality::Major,      "I",    HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Minor,      "ii",   HarmonicFunction::Subdominant },
            DegreeInfo { ChordQuality::Minor,      "iii",  HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Major,      "IV",   HarmonicFunction::Subdominant },
            DegreeInfo { ChordQuality::Major,      "V",    HarmonicFunction::Dominant },
            DegreeInfo { ChordQuality::Minor,      "vi",   HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Diminished, "vii\xC2\xB0", HarmonicFunction::Dominant },
        };

        constexpr std::array<DegreeInfo, 7> kNaturalMinorDegrees {
            DegreeInfo { ChordQuality::Minor,      "i",    HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Diminished, "ii\xC2\xB0", HarmonicFunction::Subdominant },
            DegreeInfo { ChordQuality::Major,      "III",  HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Minor,      "iv",   HarmonicFunction::Subdominant },
            DegreeInfo { ChordQuality::Minor,      "v",    HarmonicFunction::Dominant },
            DegreeInfo { ChordQuality::Major,      "VI",   HarmonicFunction::Tonic },
            DegreeInfo { ChordQuality::Major,      "VII",  HarmonicFunction::Dominant },
        };
    }

    std::array<DiatonicChord, 7> generateDiatonicChords(const Key& key)
    {
        const auto pitchClasses = key.degreePitchClasses();
        const auto& degrees = key.type == ScaleType::Major ? kMajorDegrees : kNaturalMinorDegrees;

        std::array<DiatonicChord, 7> result {};
        for (size_t i = 0; i < 7; ++i)
        {
            result[i] = DiatonicChord {
                static_cast<int>(i) + 1,
                Chord { pitchClasses[i], degrees[i].quality },
                degrees[i].romanNumeral,
                degrees[i].function
            };
        }
        return result;
    }

    std::string describe(HarmonicFunction function)
    {
        switch (function)
        {
            case HarmonicFunction::Tonic:       return "Tonic - the key's home chord. Feels resolved and stable.";
            case HarmonicFunction::Subdominant: return "Subdominant - moves away from home, builds mild tension.";
            case HarmonicFunction::Dominant:    return "Dominant - strong pull back to the Tonic.";
        }
        return {};
    }
}
