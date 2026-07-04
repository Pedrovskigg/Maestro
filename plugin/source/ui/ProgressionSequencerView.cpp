#include "ProgressionSequencerView.h"

using namespace maestro::theory;

namespace
{
    constexpr int kControlsWidth = 210;
    constexpr int kChipWidth = 64;
    constexpr int kChipGap = 6;
}

ProgressionSequencerView::ProgressionSequencerView()
{
    addAndMakeVisible(playButton);
    playButton.onClick = [this] { if (onPlayStopClicked) onPlayStopClicked(); };

    addAndMakeVisible(clearButton);
    clearButton.onClick = [this] { if (onClearClicked) onClearClicked(); };

    addAndMakeVisible(bpmLabel);
    bpmLabel.setJustificationType(juce::Justification::centred);

    bpmSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bpmSlider.setRange(40.0, 240.0, 1.0);
    bpmSlider.setValue(120.0, juce::dontSendNotification);
    bpmSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 20);
    bpmSlider.onValueChange = [this] { if (onBpmChanged) onBpmChanged(bpmSlider.getValue()); };
    addAndMakeVisible(bpmSlider);

    startTimerHz(20);
}

void ProgressionSequencerView::resized()
{
    auto bounds = getLocalBounds();
    auto controls = bounds.removeFromLeft(kControlsWidth).reduced(4);

    playButton.setBounds(controls.removeFromTop(controls.getHeight() / 2).reduced(2));
    clearButton.setBounds(controls.removeFromLeft(controls.getWidth() / 2).reduced(2));
    bpmLabel.setBounds(controls.removeFromLeft(36));
    bpmSlider.setBounds(controls.reduced(2));
}

void ProgressionSequencerView::setChords(std::vector<Chord> newChords, std::vector<juce::Colour> newColours)
{
    chords = std::move(newChords);
    colours = std::move(newColours);
    repaint();
}

void ProgressionSequencerView::setPlaying(bool isPlaying)
{
    playButton.setButtonText(isPlaying ? "Stop" : "Play");
}

void ProgressionSequencerView::timerCallback()
{
    if (!currentStepProvider)
        return;

    const int step = currentStepProvider();
    if (step != lastDisplayedStep)
    {
        lastDisplayedStep = step;
        repaint();
    }
}

void ProgressionSequencerView::paint(juce::Graphics& g)
{
    auto area = getLocalBounds();
    area.removeFromLeft(kControlsWidth + 8);
    area = area.reduced(4);

    if (chords.empty())
    {
        g.setColour(juce::Colours::lightgrey);
        g.setFont(juce::FontOptions(13.0f));
        g.drawText("Play a chord pad to build a progression", area, juce::Justification::centredLeft);
        return;
    }

    const int currentStep = currentStepProvider ? currentStepProvider() : -1;

    for (size_t i = 0; i < chords.size(); ++i)
    {
        const int x = area.getX() + static_cast<int>(i) * (kChipWidth + kChipGap);
        if (x + kChipWidth > area.getRight())
            break; // MVP: no scrolling yet, extra chips are simply not shown

        juce::Rectangle<float> chip(static_cast<float>(x), static_cast<float>(area.getY()),
                                     static_cast<float>(kChipWidth), static_cast<float>(area.getHeight()));

        const bool isCurrent = static_cast<int>(i) == currentStep;
        g.setColour(colours[i].withAlpha(isCurrent ? 0.95f : 0.55f));
        g.fillRoundedRectangle(chip, 6.0f);

        g.setColour(isCurrent ? juce::Colours::white : juce::Colours::white.withAlpha(0.6f));
        g.drawRoundedRectangle(chip, 6.0f, isCurrent ? 2.0f : 1.0f);

        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(13.0f, juce::Font::bold));
        g.drawText(juce::String(toString(chords[i])), chip.toNearestInt(), juce::Justification::centred);
    }
}
