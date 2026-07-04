#include "ProgressionTreeView.h"
#include "FunctionColours.h"
#include "maestro/theory/ChordSuggester.h"
#include "maestro/theory/DiatonicChordGenerator.h"

using namespace maestro::theory;

namespace
{
    juce::String utf8(const std::string& s)
    {
        return juce::String(juce::CharPointer_UTF8(s.c_str()));
    }

    // A chord suggested as a possible next step; expanding it shows what could follow *it*.
    class ChordTreeItem : public juce::TreeViewItem
    {
    public:
        ChordTreeItem(Key key, Chord chord, HarmonicFunction function, juce::String mainText, juce::String subtitle)
            : key(key), chord(chord), function(function), mainText(std::move(mainText)), subtitle(std::move(subtitle))
        {
        }

        bool mightContainSubItems() override { return true; }
        int getItemHeight() const override { return 40; }

        void itemOpennessChanged(bool isNowOpen) override
        {
            if (!isNowOpen || getNumSubItems() > 0)
                return;

            const auto diatonic = generateDiatonicChords(key);
            for (const auto& suggestion : suggestNextChords(key, { chord }, 4))
            {
                const auto& degreeInfo = diatonic[static_cast<size_t>(suggestion.degree - 1)];
                const juce::String label = utf8(degreeInfo.romanNumeral) + " - " + juce::String(toString(suggestion.chord));
                addSubItem(new ChordTreeItem(key, suggestion.chord, degreeInfo.function, label,
                                              juce::String(suggestion.rationale)));
            }
        }

        void paintItem(juce::Graphics& g, int width, int height) override
        {
            g.setColour(colourForFunction(function));
            g.fillRect(0, 0, 4, height);

            g.setColour(juce::Colours::white);
            g.setFont(juce::FontOptions(15.0f, juce::Font::bold));
            g.drawText(mainText, 10, 0, width - 14, height / 2, juce::Justification::centredLeft);

            if (subtitle.isNotEmpty())
            {
                g.setColour(juce::Colours::lightgrey);
                g.setFont(juce::FontOptions(12.0f));
                g.drawText(subtitle, 10, height / 2, width - 14, height / 2, juce::Justification::centredLeft);
            }
        }

    private:
        Key key;
        Chord chord;
        HarmonicFunction function;
        juce::String mainText;
        juce::String subtitle;
    };
}

class RootTreeItem : public juce::TreeViewItem
{
public:
    explicit RootTreeItem(Key key) : key(key) {}

    bool mightContainSubItems() override { return true; }

    void itemOpennessChanged(bool isNowOpen) override
    {
        if (!isNowOpen || getNumSubItems() > 0)
            return;

        for (const auto& diatonic : generateDiatonicChords(key))
        {
            const juce::String label = utf8(diatonic.romanNumeral) + " - " + juce::String(toString(diatonic.chord));
            addSubItem(new ChordTreeItem(key, diatonic.chord, diatonic.function, label,
                                          juce::String(describe(diatonic.function))));
        }
    }

private:
    Key key;
};

ProgressionTreeView::ProgressionTreeView()
{
    addAndMakeVisible(treeView);
    treeView.setRootItemVisible(false);
    treeView.setColour(juce::TreeView::backgroundColourId, juce::Colours::transparentBlack);
}

ProgressionTreeView::~ProgressionTreeView()
{
    treeView.setRootItem(nullptr);
}

void ProgressionTreeView::resized()
{
    treeView.setBounds(getLocalBounds());
}

void ProgressionTreeView::setKey(const Key& key)
{
    treeView.setRootItem(nullptr);
    rootItem = std::make_unique<RootTreeItem>(key);
    treeView.setRootItem(rootItem.get());
    rootItem->setOpen(true);
}
