#include "PluginEditor.h"
#include "ui/FunctionColours.h"
#include "maestro/theory/DiatonicChordGenerator.h"
#include <algorithm>

namespace
{
    constexpr int kKeyboardLowNote = 36;  // C2
    constexpr int kKeyboardHighNote = 96; // C7

    bool isWhiteKey(int midiNote)
    {
        switch (midiNote % 12)
        {
            case 0: case 2: case 4: case 5: case 7: case 9: case 11: return true;
            default: return false;
        }
    }

    int countWhiteKeys(int lowNote, int highNote)
    {
        int count = 0;
        for (int note = lowNote; note <= highNote; ++note)
            if (isWhiteKey(note))
                ++count;
        return count;
    }
}

MaestroAudioProcessorEditor::MaestroAudioProcessorEditor(MaestroAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), processor(p),
      keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyScaleSelector);
    addAndMakeVisible(chordPalette);
    addAndMakeVisible(progressionSequencer);
    addAndMakeVisible(harmonicWheel);
    addAndMakeVisible(keyboardComponent);

    keyboardComponent.setAvailableRange(kKeyboardLowNote, kKeyboardHighNote); // C2 to C7, exactly 5 octaves
    keyboardComponent.setLowestVisibleKey(kKeyboardLowNote);

    detectedChordLabel.setJustificationType(juce::Justification::centred);
    detectedChordLabel.setFont(juce::FontOptions(15.0f, juce::Font::bold));
    detectedChordLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.75f));
    detectedChordLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    detectedChordLabel.setInterceptsMouseClicks(false, false);
    addChildComponent(detectedChordLabel); // not addAndMakeVisible: stays hidden until a chord is detected

    keyScaleSelector.onKeyChanged = [this](maestro::theory::Key) { refreshChords(); };
    chordPalette.onChordTriggered = [this](const maestro::theory::Chord& chord, bool isNoteOn)
    {
        if (isNoteOn)
        {
            processor.triggerChordOn(chord);
            processor.addChordToProgression(chord);
            refreshProgressionDisplay();
        }
        else
        {
            processor.triggerChordOff(chord);
        }
    };

    progressionSequencer.onPlayStopClicked = [this]
    {
        const bool nowPlaying = !processor.isPlaying();
        processor.setPlaying(nowPlaying);
        progressionSequencer.setPlaying(nowPlaying);
    };
    progressionSequencer.onClearClicked = [this]
    {
        processor.setPlaying(false);
        processor.clearProgression();
        progressionSequencer.setPlaying(false);
        refreshProgressionDisplay();
    };
    progressionSequencer.onBpmChanged = [this](double newBpm) { processor.setBpm(newBpm); };
    progressionSequencer.currentStepProvider = [this] { return processor.getCurrentStepIndex(); };

    refreshChords();
    refreshProgressionDisplay();

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
    progressionSequencer.setBounds(bounds.removeFromTop(56));
    bounds.removeFromTop(8);

    keyboardComponent.setBounds(bounds.removeFromBottom(150));

    // Stretch the keys so the fixed note range exactly fills the keyboard's width - no dead space.
    const int whiteKeyCount = countWhiteKeys(kKeyboardLowNote, kKeyboardHighNote);
    if (whiteKeyCount > 0)
        keyboardComponent.setKeyWidth(static_cast<float>(keyboardComponent.getWidth()) / static_cast<float>(whiteKeyCount));

    bounds.removeFromBottom(8);
    harmonicWheel.setBounds(bounds);

    // The floating "chord you just played" balloon is positioned dynamically in showDetectedChord().
}

void MaestroAudioProcessorEditor::refreshChords()
{
    const auto key = keyScaleSelector.currentKey();
    const auto extension = keyScaleSelector.currentExtension();
    chordPalette.setChords(maestro::theory::generateDiatonicChords(key), extension);
    harmonicWheel.setKey(key, extension);
}

void MaestroAudioProcessorEditor::refreshProgressionDisplay()
{
    const auto progression = processor.getProgression();
    const auto diatonic = maestro::theory::generateDiatonicChords(keyScaleSelector.currentKey());

    std::vector<juce::Colour> colours;
    colours.reserve(progression.size());
    for (const auto& chord : progression)
    {
        const auto it = std::find_if(diatonic.begin(), diatonic.end(),
                                      [&chord](const auto& d) { return d.chord == chord; });
        colours.push_back(it != diatonic.end() ? colourForFunction(it->function) : juce::Colours::grey);
    }

    progressionSequencer.setChords(progression, colours);
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

    showDetectedChord(*detected, heldNotes);
}

void MaestroAudioProcessorEditor::showDetectedChord(const maestro::theory::Chord& chord, const std::vector<int>& heldNotes)
{
    lastDetectedChord = chord;
    detectedChordLabel.setText(juce::String(maestro::theory::toString(chord)), juce::dontSendNotification);

    // Anchor the balloon above whichever held note matches the chord's root (falls back to the
    // lowest held note if, for some reason, none matches - shouldn't normally happen).
    int anchorNote = heldNotes.front();
    for (int note : heldNotes)
    {
        if (((note % 12) + 12) % 12 == static_cast<int>(chord.root))
        {
            anchorNote = note;
            break;
        }
    }

    constexpr int balloonWidth = 70;
    constexpr int balloonHeight = 24;
    const float keyCentreX = keyboardComponent.getKeyStartPosition(anchorNote) + (keyboardComponent.getKeyWidth() * 0.5f);
    const int x = keyboardComponent.getX() + static_cast<int>(keyCentreX) - balloonWidth / 2;
    const int y = keyboardComponent.getY() - balloonHeight - 4;

    detectedChordLabel.setBounds(x, y, balloonWidth, balloonHeight);
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
