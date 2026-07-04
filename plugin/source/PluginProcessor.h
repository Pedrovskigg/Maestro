#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "maestro/theory/Chord.h"

class MaestroAudioProcessor : public juce::AudioProcessor
{
public:
    MaestroAudioProcessor();
    ~MaestroAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    // Called from the UI thread when a chord pad is pressed/released; forwards to the audio
    // thread's MIDI output via MidiKeyboardState, which handles the thread-safety for us.
    void triggerChordOn(const maestro::theory::Chord& chord);
    void triggerChordOff(const maestro::theory::Chord& chord);

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Maestro"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

private:
    static constexpr int kChordOctave = 4;

    juce::MidiKeyboardState keyboardState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaestroAudioProcessor)
};
