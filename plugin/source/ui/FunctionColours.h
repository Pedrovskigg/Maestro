#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "maestro/theory/DiatonicChordGenerator.h"

// Subtle, non-"carnavalesco" accent colour per harmonic function, shared by every chord-related
// UI element so the eye can pattern-match Tonic/Subdominant/Dominant at a glance across the app.
inline juce::Colour colourForFunction(maestro::theory::HarmonicFunction function)
{
    using maestro::theory::HarmonicFunction;
    switch (function)
    {
        case HarmonicFunction::Tonic:       return juce::Colours::mediumseagreen;
        case HarmonicFunction::Subdominant: return juce::Colours::cornflowerblue;
        case HarmonicFunction::Dominant:    return juce::Colours::indianred;
    }
    return juce::Colours::grey;
}
