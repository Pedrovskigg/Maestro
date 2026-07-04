#include "HarmonicWheelView.h"
#include "FunctionColours.h"
#include <cmath>

using namespace maestro::theory;

namespace
{
    juce::String utf8(const std::string& s)
    {
        return juce::String(juce::CharPointer_UTF8(s.c_str()));
    }
}

HarmonicWheelView::HarmonicWheelView()
{
    setKey(currentKey);
}

void HarmonicWheelView::setKey(const Key& key)
{
    currentKey = key;
    diatonicChords = generateDiatonicChords(key);
    selectDegree(1);
}

void HarmonicWheelView::selectDegree(int degree)
{
    selectedDegree = degree;
    const auto& chord = diatonicChords[static_cast<size_t>(degree - 1)].chord;
    currentSuggestions = suggestNextChords(currentKey, { chord }, 4);
    repaint();
}

juce::Rectangle<float> HarmonicWheelView::getWheelArea() const
{
    auto bounds = getLocalBounds();
    return bounds.removeFromLeft(static_cast<int>(getWidth() * 0.6f)).toFloat();
}

std::array<HarmonicWheelView::NodePosition, 7> HarmonicWheelView::layoutNodes() const
{
    std::array<NodePosition, 7> nodes;
    const auto area = getWheelArea();
    const juce::Point<float> centre = area.getCentre();
    const float wheelRadius = juce::jmin(area.getWidth(), area.getHeight()) * 0.5f - 40.0f;
    const float nodeRadius = 32.0f;

    for (int i = 0; i < 7; ++i)
    {
        const float angle = -juce::MathConstants<float>::halfPi + static_cast<float>(i) * (juce::MathConstants<float>::twoPi / 7.0f);
        const juce::Point<float> p { centre.x + wheelRadius * std::cos(angle), centre.y + wheelRadius * std::sin(angle) };
        nodes[static_cast<size_t>(i)] = { p, nodeRadius };
    }
    return nodes;
}

void HarmonicWheelView::paint(juce::Graphics& g)
{
    const auto nodes = layoutNodes();

    if (selectedDegree >= 1)
    {
        const auto& fromPos = nodes[static_cast<size_t>(selectedDegree - 1)].centre;
        for (const auto& suggestion : currentSuggestions)
        {
            const auto& toPos = nodes[static_cast<size_t>(suggestion.degree - 1)].centre;
            const float weight = static_cast<float>(suggestion.weight);
            g.setColour(juce::Colours::white.withAlpha(0.25f + weight * 0.6f));
            g.drawLine(juce::Line<float>(fromPos, toPos), 1.0f + weight * 3.5f);
        }
    }

    for (int i = 0; i < 7; ++i)
    {
        const auto& diatonic = diatonicChords[static_cast<size_t>(i)];
        const auto& node = nodes[static_cast<size_t>(i)];
        const bool isSelected = (i + 1) == selectedDegree;
        auto nodeBounds = juce::Rectangle<float>(node.radius * 2.0f, node.radius * 2.0f).withCentre(node.centre);

        g.setColour(colourForFunction(diatonic.function).withAlpha(isSelected ? 0.9f : 0.5f));
        g.fillEllipse(nodeBounds);

        g.setColour(isSelected ? juce::Colours::white : juce::Colours::white.withAlpha(0.7f));
        g.drawEllipse(nodeBounds, isSelected ? 2.5f : 1.0f);

        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(13.0f, juce::Font::bold));
        g.drawText(utf8(diatonic.romanNumeral), nodeBounds.removeFromTop(nodeBounds.getHeight() * 0.55f),
                    juce::Justification::centred);
        g.setFont(juce::FontOptions(11.0f));
        g.drawText(juce::String(toString(diatonic.chord)), nodeBounds, juce::Justification::centred);
    }

    auto infoArea = getLocalBounds();
    infoArea.removeFromLeft(static_cast<int>(getWidth() * 0.6f));
    infoArea = infoArea.reduced(10);

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    const auto& selectedChord = diatonicChords[static_cast<size_t>(selectedDegree - 1)];
    g.drawText("From " + utf8(selectedChord.romanNumeral) + " - " + juce::String(toString(selectedChord.chord)),
               infoArea.removeFromTop(24), juce::Justification::centredLeft);

    for (const auto& suggestion : currentSuggestions)
    {
        auto row = infoArea.removeFromTop(46);
        const auto& targetFunction = diatonicChords[static_cast<size_t>(suggestion.degree - 1)].function;

        g.setColour(colourForFunction(targetFunction));
        g.fillRect(row.removeFromLeft(4));
        row.removeFromLeft(6);

        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(13.0f, juce::Font::bold));
        g.drawText(juce::String(toString(suggestion.chord)), row.removeFromTop(18), juce::Justification::centredLeft);

        g.setColour(juce::Colours::lightgrey);
        g.setFont(juce::FontOptions(11.0f));
        g.drawFittedText(juce::String(suggestion.rationale), row, juce::Justification::topLeft, 2);
    }
}

void HarmonicWheelView::resized()
{
}

void HarmonicWheelView::mouseDown(const juce::MouseEvent& e)
{
    const auto nodes = layoutNodes();
    for (int i = 0; i < 7; ++i)
    {
        if (nodes[static_cast<size_t>(i)].centre.getDistanceFrom(e.position) <= nodes[static_cast<size_t>(i)].radius)
        {
            selectDegree(i + 1);
            return;
        }
    }
}
