#include "KeyScaleSelector.h"

using namespace maestro::theory;

namespace
{
    constexpr const char* kRootNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
}

KeyScaleSelector::KeyScaleSelector()
{
    for (int i = 0; i < 12; ++i)
        rootBox.addItem(kRootNames[i], i + 1);
    rootBox.setSelectedId(1, juce::dontSendNotification);
    rootBox.onChange = [this] { notifyChange(); };
    addAndMakeVisible(rootBox);

    scaleTypeBox.addItem("Major", 1);
    scaleTypeBox.addItem("Natural minor", 2);
    scaleTypeBox.setSelectedId(1, juce::dontSendNotification);
    scaleTypeBox.onChange = [this] { notifyChange(); };
    addAndMakeVisible(scaleTypeBox);
}

void KeyScaleSelector::resized()
{
    auto bounds = getLocalBounds();
    rootBox.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2).reduced(4));
    scaleTypeBox.setBounds(bounds.reduced(4));
}

Key KeyScaleSelector::currentKey() const
{
    const auto root = static_cast<PitchClass>(rootBox.getSelectedId() - 1);
    const auto type = scaleTypeBox.getSelectedId() == 2 ? ScaleType::NaturalMinor : ScaleType::Major;
    return Key { root, type };
}

void KeyScaleSelector::notifyChange()
{
    if (onKeyChanged)
        onKeyChanged(currentKey());
}
