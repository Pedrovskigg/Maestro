#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace maestro::theory;

MaestroAudioProcessor::MaestroAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

void MaestroAudioProcessor::prepareToPlay(double newSampleRate, int)
{
    sampleRate = newSampleRate;
}

void MaestroAudioProcessor::releaseResources()
{
}

void MaestroAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    if (playing.load())
    {
        const juce::ScopedLock lock(progressionLock);
        if (!progression.empty())
        {
            samplesUntilNextStep -= buffer.getNumSamples();
            if (samplesUntilNextStep <= 0)
                advanceStep();
        }
    }
}

void MaestroAudioProcessor::advanceStep()
{
    if (stepIndexInternal >= 0 && stepIndexInternal < static_cast<int>(progression.size()))
        for (int note : chordMidiNotes(progression[static_cast<size_t>(stepIndexInternal)], kChordOctave))
            keyboardState.noteOff(1, note, 0.0f);

    stepIndexInternal = (stepIndexInternal + 1) % static_cast<int>(progression.size());
    currentStepIndex.store(stepIndexInternal);

    for (int note : chordMidiNotes(progression[static_cast<size_t>(stepIndexInternal)], kChordOctave))
        keyboardState.noteOn(1, note, 0.8f);

    const double secondsPerChord = kBeatsPerChord * 60.0 / bpm.load();
    samplesUntilNextStep += static_cast<int64_t>(secondsPerChord * sampleRate);
}

void MaestroAudioProcessor::triggerChordOn(const Chord& chord)
{
    for (int note : chordMidiNotes(chord, kChordOctave))
        keyboardState.noteOn(1, note, 0.8f);
}

void MaestroAudioProcessor::triggerChordOff(const Chord& chord)
{
    for (int note : chordMidiNotes(chord, kChordOctave))
        keyboardState.noteOff(1, note, 0.0f);
}

void MaestroAudioProcessor::addChordToProgression(const Chord& chord)
{
    const juce::ScopedLock lock(progressionLock);
    progression.push_back(chord);
}

void MaestroAudioProcessor::clearProgression()
{
    const juce::ScopedLock lock(progressionLock);
    if (stepIndexInternal >= 0 && stepIndexInternal < static_cast<int>(progression.size()))
        for (int note : chordMidiNotes(progression[static_cast<size_t>(stepIndexInternal)], kChordOctave))
            keyboardState.noteOff(1, note, 0.0f);

    progression.clear();
    stepIndexInternal = -1;
    currentStepIndex.store(-1);
}

std::vector<Chord> MaestroAudioProcessor::getProgression() const
{
    const juce::ScopedLock lock(progressionLock);
    return progression;
}

void MaestroAudioProcessor::setPlaying(bool shouldPlay)
{
    const juce::ScopedLock lock(progressionLock);

    if (!shouldPlay && stepIndexInternal >= 0 && stepIndexInternal < static_cast<int>(progression.size()))
        for (int note : chordMidiNotes(progression[static_cast<size_t>(stepIndexInternal)], kChordOctave))
            keyboardState.noteOff(1, note, 0.0f);

    playing.store(shouldPlay);
    stepIndexInternal = -1;
    samplesUntilNextStep = 0;
    currentStepIndex.store(-1);
}

juce::AudioProcessorEditor* MaestroAudioProcessor::createEditor()
{
    return new MaestroAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaestroAudioProcessor();
}
