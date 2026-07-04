#pragma once

#include <functional>
#include <vector>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/Chord.h"

// Shows the recorded chord progression as a row of chips, with Play/Stop, Clear and a BPM
// control. Playback itself happens sample-accurately in MaestroAudioProcessor; this view just
// displays the chips and polls the processor for the currently-playing step to highlight it.
class ProgressionSequencerView : public juce::Component, private juce::Timer
{
public:
    ProgressionSequencerView();

    void paint(juce::Graphics&) override;
    void resized() override;

    void setChords(std::vector<maestro::theory::Chord> newChords, std::vector<juce::Colour> newColours);
    void setPlaying(bool isPlaying);

    std::function<void()> onPlayStopClicked;
    std::function<void()> onClearClicked;
    std::function<void(double)> onBpmChanged;
    std::function<int()> currentStepProvider; // returns the processor's current playing step, or -1

private:
    juce::TextButton playButton { "Play" };
    juce::TextButton clearButton { "Clear" };
    juce::Slider bpmSlider;
    juce::Label bpmLabel { {}, "BPM" };

    std::vector<maestro::theory::Chord> chords;
    std::vector<juce::Colour> colours;
    int lastDisplayedStep = -2;

    void timerCallback() override;
};
