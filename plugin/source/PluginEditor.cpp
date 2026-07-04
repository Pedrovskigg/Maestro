#include "PluginEditor.h"
#include "maestro/theory/DiatonicChordGenerator.h"

MaestroAudioProcessorEditor::MaestroAudioProcessorEditor(MaestroAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    addAndMakeVisible(keyScaleSelector);
    addAndMakeVisible(chordPalette);

    keyScaleSelector.onKeyChanged = [this](maestro::theory::Key) { refreshChords(); };
    chordPalette.onChordTriggered = [this](const maestro::theory::Chord& chord, bool isNoteOn)
    {
        if (isNoteOn)
            processor.triggerChordOn(chord);
        else
            processor.triggerChordOff(chord);
    };
    refreshChords();

    setSize(600, 400);
}

void MaestroAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MaestroAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(8);
    keyScaleSelector.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(8);
    chordPalette.setBounds(bounds.removeFromTop(80));
}

void MaestroAudioProcessorEditor::refreshChords()
{
    const auto key = keyScaleSelector.currentKey();
    chordPalette.setChords(maestro::theory::generateDiatonicChords(key));
}
