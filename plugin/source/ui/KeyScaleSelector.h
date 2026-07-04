#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/Key.h"
#include "maestro/theory/Chord.h"

class KeyScaleSelector : public juce::Component
{
public:
    KeyScaleSelector();

    void resized() override;

    maestro::theory::Key currentKey() const;
    maestro::theory::ChordExtension currentExtension() const;

    // Fires when the root, scale type, or chord type (extension) changes.
    std::function<void(maestro::theory::Key)> onKeyChanged;

private:
    juce::ComboBox rootBox;
    juce::ComboBox scaleTypeBox;
    juce::ComboBox chordTypeBox;

    void notifyChange();
};
