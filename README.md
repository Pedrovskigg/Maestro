# Maestro

Plugin de áudio (VST3) gratuito e open-source de composição musical assistida. O usuário escolhe tônica, escala e vai montando progressões de acordes sem precisar de conhecimento prévio de teoria musical — o Maestro gera e sugere a teoria (acordes diatônicos, próximos acordes prováveis) conforme as escolhas são feitas.

Inspirado em ferramentas como Scaler 2, Captain Chords e Komposer.

## Status

MVP em desenvolvimento. Escopo da v1: seleção de tonalidade/escala, acordes diatônicos e sugestão de próximo acorde. Geração de melodia fica para uma versão futura.

Plataforma inicial: Windows / VST3. Suporte a macOS/AU é planejado, mas não é o foco da v1.

## Estrutura do repositório

- `libs/music_theory/` — motor de teoria musical em C++ puro, sem dependência do JUCE. Testável isoladamente via CTest.
- `plugin/` — plugin JUCE (AudioProcessor + UI) que consome `music_theory`.
- `external/JUCE/` — framework JUCE, como git submodule.
- `docs/` — documentação de arquitetura e atribuições de licença de terceiros.

## Build

Requisitos: CMake ≥ 3.22, MSVC (Visual Studio 2022 Build Tools ou superior, workload de Desktop C++).

```
git clone --recurse-submodules https://github.com/Pedrovskigg/Maestro.git
cmake -B build
cmake --build build --config Release
```

## Testes

```
ctest --test-dir build
```

## Licença

Código do Maestro sob [GPL-3.0](LICENSE). Veja [docs/LICENSE-THIRD-PARTY.md](docs/LICENSE-THIRD-PARTY.md) para as licenças de dependências de terceiros (JUCE, VST3 SDK).

## Roadmap

- **v1**: escalas, acordes diatônicos e sugestão de progressão.
- **v2**: geração/sugestão de melodia.
