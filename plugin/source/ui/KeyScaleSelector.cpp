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

    chordTypeBox.addItem("Triads", 1);
    chordTypeBox.addItem("7th chords", 2);
    chordTypeBox.addItem("Add9", 3);
    chordTypeBox.addItem("Power chords (5)", 4);
    chordTypeBox.setSelectedId(1, juce::dontSendNotification);
    chordTypeBox.onChange = [this] { notifyChange(); };
    addAndMakeVisible(chordTypeBox);
}

void KeyScaleSelector::resized()
{
    auto bounds = getLocalBounds();
    const int thirdWidth = bounds.getWidth() / 3;
    rootBox.setBounds(bounds.removeFromLeft(thirdWidth).reduced(4));
    scaleTypeBox.setBounds(bounds.removeFromLeft(thirdWidth).reduced(4));
    chordTypeBox.setBounds(bounds.reduced(4));
}

Key KeyScaleSelector::currentKey() const
{
    const auto root = static_cast<PitchClass>(rootBox.getSelectedId() - 1);
    const auto type = scaleTypeBox.getSelectedId() == 2 ? ScaleType::NaturalMinor : ScaleType::Major;
    return Key { root, type };
}

ChordExtension KeyScaleSelector::currentExtension() const
{
    switch (chordTypeBox.getSelectedId())
    {
        case 2: return ChordExtension::Seventh;
        case 3: return ChordExtension::Add9;
        case 4: return ChordExtension::PowerFifth;
        default: return ChordExtension::Triad;
    }
}

void KeyScaleSelector::notifyChange()
{
    if (onKeyChanged)
        onKeyChanged(currentKey());
}
