#include "PluginProcessor.h"
#include "PluginEditor.h"

MaestroAudioProcessor::MaestroAudioProcessor()
{
}

void MaestroAudioProcessor::prepareToPlay(double, int)
{
}

void MaestroAudioProcessor::releaseResources()
{
}

void MaestroAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&)
{
}

juce::AudioProcessorEditor* MaestroAudioProcessor::createEditor()
{
    return new MaestroAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaestroAudioProcessor();
}
