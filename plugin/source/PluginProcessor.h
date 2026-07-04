#pragma once

#include <atomic>
#include <vector>
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

    juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }

    // Progression sequencer: a simple loop of chords, one per bar, played back sample-accurately
    // from processBlock so it can drive a downstream instrument in the host without drift.
    void addChordToProgression(const maestro::theory::Chord& chord);
    void clearProgression();
    std::vector<maestro::theory::Chord> getProgression() const;

    void setPlaying(bool shouldPlay);
    bool isPlaying() const { return playing.load(); }

    void setBpm(double newBpm) { bpm.store(juce::jlimit(20.0, 300.0, newBpm)); }
    double getBpm() const { return bpm.load(); }

    int getCurrentStepIndex() const { return currentStepIndex.load(); }

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
    static constexpr double kBeatsPerChord = 4.0; // one bar per chord

    juce::MidiKeyboardState keyboardState;

    mutable juce::CriticalSection progressionLock;
    std::vector<maestro::theory::Chord> progression;
    int stepIndexInternal = -1; // audio-thread-owned; guarded by progressionLock
    int64_t samplesUntilNextStep = 0;
    double sampleRate = 44100.0;

    std::atomic<bool> playing { false };
    std::atomic<double> bpm { 120.0 };
    std::atomic<int> currentStepIndex { -1 };

    // Assumes progressionLock is already held by the caller.
    void advanceStep();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaestroAudioProcessor)
};
