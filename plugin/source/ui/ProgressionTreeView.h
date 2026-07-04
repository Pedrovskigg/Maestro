#pragma once

#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/Key.h"

class RootTreeItem;

// An explorable tree of chord progressions: the top level lists the key's 7 diatonic chords;
// expanding any chord lazily shows its likely next chords (from ChordSuggester), which can in
// turn be expanded further, letting the user build out a progression branch by branch.
class ProgressionTreeView : public juce::Component
{
public:
    ProgressionTreeView();
    ~ProgressionTreeView() override;

    void resized() override;

    void setKey(const maestro::theory::Key& key);

private:
    juce::TreeView treeView;
    std::unique_ptr<RootTreeItem> rootItem;
};
