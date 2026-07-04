#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/Key.h"

class KeyScaleSelector : public juce::Component
{
public:
    KeyScaleSelector();

    void resized() override;

    maestro::theory::Key currentKey() const;

    std::function<void(maestro::theory::Key)> onKeyChanged;

private:
    juce::ComboBox rootBox;
    juce::ComboBox scaleTypeBox;

    void notifyChange();
};
