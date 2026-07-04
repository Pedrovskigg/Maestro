#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class MaestroAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MaestroAudioProcessorEditor(MaestroAudioProcessor&);
    ~MaestroAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MaestroAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaestroAudioProcessorEditor)
};
