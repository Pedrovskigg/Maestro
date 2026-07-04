#include "PluginEditor.h"

MaestroAudioProcessorEditor::MaestroAudioProcessorEditor(MaestroAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    setSize(600, 400);
}

void MaestroAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(24.0f);
    g.drawFittedText("Maestro", getLocalBounds(), juce::Justification::centred, 1);
}

void MaestroAudioProcessorEditor::resized()
{
}
