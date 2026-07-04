#pragma once

#include <optional>
#include <vector>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"
#include "ui/KeyScaleSelector.h"
#include "ui/DiatonicChordPalette.h"
#include "maestro/theory/Chord.h"

class MaestroAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     private juce::MidiKeyboardState::Listener,
                                     private juce::Timer
{
public:
    explicit MaestroAudioProcessorEditor(MaestroAudioProcessor&);
    ~MaestroAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MaestroAudioProcessor& processor;

    KeyScaleSelector keyScaleSelector;
    DiatonicChordPalette chordPalette;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::Label detectedChordLabel;
    juce::TooltipWindow tooltipWindow { this };

    std::optional<maestro::theory::Chord> lastDetectedChord;

    void refreshChords();
    void notesChanged();
    void showDetectedChord(const maestro::theory::Chord& chord, const std::vector<int>& heldNotes);

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MaestroAudioProcessorEditor)
};
