#pragma once

#include <array>
#include <functional>
#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/DiatonicChordGenerator.h"

class DiatonicChordPalette : public juce::Component
{
public:
    DiatonicChordPalette();

    void resized() override;

    void setChords(const std::array<maestro::theory::DiatonicChord, 7>& chords,
                    maestro::theory::ChordExtension extension);

    // Fired when a chord pad is pressed (isNoteOn == true) or released (isNoteOn == false).
    std::function<void(const maestro::theory::Chord&, bool isNoteOn)> onChordTriggered;

private:
    std::array<std::unique_ptr<juce::TextButton>, 7> buttons;
    std::array<maestro::theory::Chord, 7> currentChords {};
    std::array<bool, 7> buttonWasDown {};
};
