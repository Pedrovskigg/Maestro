#include "PluginProcessor.h"
#include "PluginEditor.h"

MaestroAudioProcessor::MaestroAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

void MaestroAudioProcessor::prepareToPlay(double, int)
{
}

void MaestroAudioProcessor::releaseResources()
{
}

void MaestroAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
}

void MaestroAudioProcessor::triggerChordOn(const maestro::theory::Chord& chord)
{
    for (int note : maestro::theory::triadMidiNotes(chord, kChordOctave))
        keyboardState.noteOn(1, note, 0.8f);
}

void MaestroAudioProcessor::triggerChordOff(const maestro::theory::Chord& chord)
{
    for (int note : maestro::theory::triadMidiNotes(chord, kChordOctave))
        keyboardState.noteOff(1, note, 0.0f);
}

juce::AudioProcessorEditor* MaestroAudioProcessor::createEditor()
{
    return new MaestroAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaestroAudioProcessor();
}
