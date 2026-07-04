#pragma once

#include <array>
#include <vector>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/DiatonicChordGenerator.h"
#include "maestro/theory/ChordSuggester.h"

// A circular "harmonic field clock": the key's 7 diatonic chords placed around a wheel.
// Clicking one selects it and draws lines to its likely next chords (from ChordSuggester),
// line thickness/opacity following the suggestion weight, with a text legend alongside.
class HarmonicWheelView : public juce::Component
{
public:
    HarmonicWheelView();

    void setKey(const maestro::theory::Key& key);

    void paint(juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent&) override;

private:
    struct NodePosition
    {
        juce::Point<float> centre;
        float radius;
    };

    maestro::theory::Key currentKey { maestro::theory::PitchClass::C, maestro::theory::ScaleType::Major };
    std::array<maestro::theory::DiatonicChord, 7> diatonicChords {};
    int selectedDegree = 1;
    std::vector<maestro::theory::ChordSuggestion> currentSuggestions;

    juce::Rectangle<float> getWheelArea() const;
    std::array<NodePosition, 7> layoutNodes() const;
    void selectDegree(int degree);
};
