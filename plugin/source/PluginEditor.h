#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "ui/KeyScaleSelector.h"
#include "ui/DiatonicChordPalette.h"

class MaestroAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MaestroAudioProcessorEditor(MaestroAudioProcessor&);
    ~MaestroAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MaestroAudioProcessor& processor;

    KeyScaleSelector keyScaleSelector;
    DiatonicChordPalette chordPalette;

    void refreshChords();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaestroAudioProcessorEditor)
};
