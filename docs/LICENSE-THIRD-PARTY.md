# Third-party licenses

Maestro's own code is licensed under [GPL-3.0](../LICENSE). This project depends on the third-party components below, each under its own license.

## JUCE Framework

- Location: `external/JUCE` (git submodule, tag `8.0.14`)
- License: dual AGPLv3 / [JUCE 8 commercial license](https://juce.com/legal/juce-8-licence/) — see [external/JUCE/LICENSE.md](../external/JUCE/LICENSE.md)
- Maestro uses JUCE under the **AGPLv3** terms, without a paid commercial license.

AGPLv3 is compatible with GPL-3.0 when combined into a single binary (the FSF treats this combination as permitted), so distributing Maestro (GPL-3.0) linked against JUCE (AGPLv3) is consistent. Any redistribution of Maestro must preserve this attribution.

## VST3 SDK (Steinberg)

- Location (inside the JUCE submodule): `external/JUCE/modules/juce_audio_processors_headless/format_types/VST3_SDK/`
- License: **MIT** (c) 2025 Steinberg Media Technologies GmbH — see the corresponding [LICENSE.txt](../external/JUCE/modules/juce_audio_processors_headless/format_types/VST3_SDK/LICENSE.txt)
- No proprietary agreement or GPL licensing is required to distribute a VST3 binary.
- "VST" is a registered trademark of Steinberg Media Technologies GmbH; use of the name/logo follows Steinberg's trademark guidelines, independent of the code license.

## Other JUCE dependencies relevant to Maestro

See the full list in [external/JUCE/LICENSE.md](../external/JUCE/LICENSE.md). No other third-party JUCE dependency is used directly by Maestro at this stage (we don't use AAX, ASIO, or AudioUnitSDK in the Windows/VST3 MVP).
