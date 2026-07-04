#include "DiatonicChordPalette.h"
#include "FunctionColours.h"

using namespace maestro::theory;

DiatonicChordPalette::DiatonicChordPalette()
{
    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i] = std::make_unique<juce::TextButton>();
        auto* button = buttons[i].get();

        button->onStateChange = [this, i]
        {
            const bool isDown = buttons[i]->isDown();
            if (isDown == buttonWasDown[i])
                return;

            buttonWasDown[i] = isDown;
            if (onChordTriggered)
                onChordTriggered(currentChords[i], isDown);
        };

        addAndMakeVisible(*button);
    }
}

void DiatonicChordPalette::resized()
{
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    for (auto& button : buttons)
        flexBox.items.add(juce::FlexItem(*button).withFlex(1.0f).withMargin(4.0f));

    flexBox.performLayout(getLocalBounds());
}

void DiatonicChordPalette::setChords(const std::array<DiatonicChord, 7>& chords, ChordExtension extension)
{
    for (size_t i = 0; i < chords.size(); ++i)
    {
        const auto& diatonic = chords[i];

        Chord displayChord = extension == ChordExtension::Seventh ? withSeventh(diatonic) : diatonic.chord;
        if (extension != ChordExtension::Seventh)
            displayChord.extension = extension;

        const juce::String roman = juce::String(juce::CharPointer_UTF8(diatonic.romanNumeral.c_str()));
        const juce::String chordName = juce::String(toString(displayChord));
        buttons[i]->setButtonText(roman + " - " + chordName);
        buttons[i]->setTooltip(roman + " (" + chordName + ") - " + describe(diatonic.function));
        buttons[i]->setColour(juce::TextButton::buttonColourId, colourForFunction(diatonic.function).withAlpha(0.18f));
        currentChords[i] = displayChord;
    }
}
