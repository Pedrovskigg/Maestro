#pragma once

#include <array>
#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/DiatonicChordGenerator.h"

class DiatonicChordPalette : public juce::Component
{
public:
    DiatonicChordPalette();

    void resized() override;

    void setChords(const std::array<maestro::theory::DiatonicChord, 7>& chords);

private:
    std::array<std::unique_ptr<juce::TextButton>, 7> buttons;
};
