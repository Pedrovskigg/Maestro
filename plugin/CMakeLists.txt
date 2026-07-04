juce_add_plugin(Maestro
    COMPANY_NAME "Maestro Project"
    BUNDLE_ID "com.maestroproject.maestro"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
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
)

target_include_directories(Maestro PRIVATE source)

target_compile_definitions(Maestro PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
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
