# Maestro

A free, open-source VST3 plugin for assisted music composition. Pick a key and scale, build chord progressions, and let Maestro explain and suggest the theory behind every choice — no music theory background required going in, and it teaches you as you go.

Inspired by tools like Scaler 2, Captain Chords, and Komposer.

## Status

MVP in development. v1 scope: key/scale selection, diatonic chords, and next-chord suggestions with plain-language explanations. Melody generation is planned for a later version.

Initial platform: Windows / VST3. macOS/AU support is planned but not the focus of v1.

## Why

Most composition-assistant plugins just hand you notes and chords with no explanation, so you stay dependent on the tool forever. Maestro is designed to teach music theory *while* you use it — every suggestion comes with a short explanation of why it works (tonic/subdominant/dominant function, common cadences, etc.), so you get progressively less dependent on it as you actually learn.

## Repository layout

- `libs/music_theory/` — pure C++ music theory engine, no JUCE dependency. Testable in isolation via CTest.
- `plugin/` — the JUCE plugin (AudioProcessor + UI) that consumes `music_theory`.
- `external/JUCE/` — the JUCE framework, as a git submodule.
- `docs/` — architecture notes and third-party license attributions.

## Building

Requirements: CMake >= 3.22, MSVC (Visual Studio 2022 Build Tools or newer, Desktop C++ workload).

```
git clone --recurse-submodules https://github.com/Pedrovskigg/Maestro.git
cmake -B build
cmake --build build --config Release
```

## Tests

```
ctest --test-dir build
```

## License

Maestro's code is licensed under [GPL-3.0](LICENSE). See [docs/LICENSE-THIRD-PARTY.md](docs/LICENSE-THIRD-PARTY.md) for third-party dependency licenses (JUCE, VST3 SDK).

## Roadmap

- **v1**: scales, diatonic chords, and progression suggestions with explanations.
- **v2**: melody generation/suggestion.
