#include "PluginEditor.h"
#include "maestro/theory/DiatonicChordGenerator.h"

MaestroAudioProcessorEditor::MaestroAudioProcessorEditor(MaestroAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), processor(p),
      keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyScaleSelector);
    addAndMakeVisible(chordPalette);
    addAndMakeVisible(keyboardComponent);

    keyboardComponent.setAvailableRange(36, 96);
    keyboardComponent.setLowestVisibleKey(48);

    detectedChordLabel.setJustificationType(juce::Justification::centred);
    detectedChordLabel.setFont(juce::FontOptions(22.0f, juce::Font::bold));
    detectedChordLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.6f));
    detectedChordLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addChildComponent(detectedChordLabel); // not addAndMakeVisible: stays hidden until a chord is detected

    keyScaleSelector.onKeyChanged = [this](maestro::theory::Key) { refreshChords(); };
    chordPalette.onChordTriggered = [this](const maestro::theory::Chord& chord, bool isNoteOn)
    {
        if (isNoteOn)
            processor.triggerChordOn(chord);
        else
            processor.triggerChordOff(chord);
    };
    refreshChords();

    processor.getKeyboardState().addListener(this);

    setSize(900, 640);
}

MaestroAudioProcessorEditor::~MaestroAudioProcessorEditor()
{
    processor.getKeyboardState().removeListener(this);
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
    chordPalette.setBounds(bounds.removeFromTop(90));
    bounds.removeFromTop(8);

    keyboardComponent.setBounds(bounds.removeFromBottom(150));
    bounds.removeFromBottom(8);

    // Remaining middle area is reserved for the next-chord suggestion panel (coming next);
    // for now it just hosts the floating "chord you just played" popup.
    detectedChordLabel.setBounds(bounds.removeFromTop(48).withSizeKeepingCentre(260, 40));
}

void MaestroAudioProcessorEditor::refreshChords()
{
    const auto key = keyScaleSelector.currentKey();
    chordPalette.setChords(maestro::theory::generateDiatonicChords(key));
}

void MaestroAudioProcessorEditor::notesChanged()
{
    std::vector<int> heldNotes;
    for (int note = 0; note < 128; ++note)
        if (processor.getKeyboardState().isNoteOnForChannels(0xffff, note))
            heldNotes.push_back(note);

    if (heldNotes.empty())
    {
        lastDetectedChord.reset();
        return;
    }

    const auto detected = maestro::theory::recognizeChord(heldNotes);
    if (!detected.has_value() || detected == lastDetectedChord)
        return;

    lastDetectedChord = detected;
    detectedChordLabel.setText("You played: " + juce::String(maestro::theory::toString(*detected)),
                                juce::dontSendNotification);
    detectedChordLabel.setVisible(true);
    startTimer(1500);
}

void MaestroAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int, int, float)
{
    // May be called from the audio thread (real MIDI input), so hop to the message thread
    // before touching any UI components.
    juce::Component::SafePointer<MaestroAudioProcessorEditor> safeThis(this);
    juce::MessageManager::callAsync([safeThis] { if (safeThis != nullptr) safeThis->notesChanged(); });
}

void MaestroAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int, int, float)
{
    juce::Component::SafePointer<MaestroAudioProcessorEditor> safeThis(this);
    juce::MessageManager::callAsync([safeThis] { if (safeThis != nullptr) safeThis->notesChanged(); });
}

void MaestroAudioProcessorEditor::timerCallback()
{
    detectedChordLabel.setVisible(false);
    stopTimer();
}
