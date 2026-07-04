#include <catch2/catch_test_macros.hpp>
#include "maestro/theory/ChordSuggester.h"

using namespace maestro::theory;

TEST_CASE("empty progression suggests tonic-function chords first", "[suggester]")
{
    Key key { PitchClass::C, ScaleType::Major };
    const auto suggestions = suggestNextChords(key, {});

    REQUIRE(!suggestions.empty());
    REQUIRE(suggestions[0].degree == 1);
    REQUIRE(suggestions[0].chord.quality == ChordQuality::Major);
    REQUIRE(suggestions[0].weight == 1.0);
}

TEST_CASE("dominant strongly suggests resolving to the tonic", "[suggester]")
{
    Key key { PitchClass::C, ScaleType::Major };
    Chord dominant { PitchClass::G, ChordQuality::Major }; // V in C major

    const auto suggestions = suggestNextChords(key, { dominant });

    REQUIRE(!suggestions.empty());
    REQUIRE(suggestions[0].degree == 1);
    REQUIRE(suggestions[0].chord.root == PitchClass::C);
}

TEST_CASE("a non-diatonic last chord falls back to tonic suggestions", "[suggester]")
{
    Key key { PitchClass::C, ScaleType::Major };
    Chord foreign { PitchClass::CSharp, ChordQuality::Major }; // not diatonic to C major

    const auto suggestions = suggestNextChords(key, { foreign });

    REQUIRE(!suggestions.empty());
    REQUIRE(suggestions[0].degree == 1);
}

TEST_CASE("maxResults caps the number of suggestions", "[suggester]")
{
    Key key { PitchClass::C, ScaleType::Major };
    Chord tonic { PitchClass::C, ChordQuality::Major };

    const auto suggestions = suggestNextChords(key, { tonic }, 2);

    REQUIRE(suggestions.size() == 2);
}
