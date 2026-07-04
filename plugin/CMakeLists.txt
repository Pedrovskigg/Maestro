juce_add_plugin(Maestro
    COMPANY_NAME "Maestro Project"
    BUNDLE_ID "com.maestroproject.maestro"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_COPY_DIR "$ENV{LOCALAPPDATA}/Programs/Common/VST3"
    PLUGIN_MANUFACTURER_CODE Mstr
    PLUGIN_CODE Mst1
    FORMATS Standalone VST3
    PRODUCT_NAME "Maestro"
)

target_sources(Maestro PRIVATE
    source/PluginProcessor.cpp
    source/PluginEditor.cpp
    source/ui/KeyScaleSelector.cpp
    source/ui/DiatonicChordPalette.cpp
    source/ui/HarmonicWheelView.cpp
    source/ui/ProgressionSequencerView.cpp
)

target_include_directories(Maestro PRIVATE source)

target_compile_definitions(Maestro PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    # JUCE only derives the VST3 category from IS_SYNTH (defaulting non-synths to the generic "Fx"
    # category) and completely ignores IS_MIDI_EFFECT for VST3. "Fx|Instrument" is Steinberg's own
    # category for "Fx which could be loaded as Instrument too" - the one MIDI-generator plugins
    # like Scaler/Captain Chords use so hosts treat them as an instrument slot (fed MIDI, chainable
    # with a real synth), instead of a plain audio effect that never receives the track's MIDI.
    JucePlugin_Vst3Category="Fx|Instrument"
)

target_link_libraries(Maestro PRIVATE
    maestro_theory
    juce::juce_audio_utils
    juce::juce_audio_processors
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
