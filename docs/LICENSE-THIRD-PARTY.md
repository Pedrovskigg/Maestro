# Licenças de terceiros

O código do Maestro em si é licenciado sob [GPL-3.0](../LICENSE). Este projeto depende dos componentes de terceiros abaixo, cada um com sua própria licença.

## JUCE Framework

- Local: `external/JUCE` (git submodule, tag `8.0.14`)
- Licença: dual AGPLv3 / [licença comercial JUCE 8](https://juce.com/legal/juce-8-licence/) — ver [external/JUCE/LICENSE.md](../external/JUCE/LICENSE.md)
- O Maestro usa o JUCE sob os termos da **AGPLv3**, sem licença comercial paga.

A AGPLv3 é compatível com a GPL-3.0 quando combinadas em um único binário (a FSF trata essa combinação como permitida), então distribuir o Maestro (GPL-3.0) linkado com o JUCE (AGPLv3) é consistente. Qualquer redistribuição do Maestro deve preservar essa atribuição.

## VST3 SDK (Steinberg)

- Local (dentro do submodule JUCE): `external/JUCE/modules/juce_audio_processors_headless/format_types/VST3_SDK/`
- Licença: **MIT** (c) 2025 Steinberg Media Technologies GmbH — ver [LICENSE.txt](../external/JUCE/modules/juce_audio_processors_headless/format_types/VST3_SDK/LICENSE.txt) correspondente
- Não exige acordo proprietário nem licenciamento GPL para distribuir binário VST3.
- "VST" é marca registrada da Steinberg Media Technologies GmbH; o uso do nome/logo segue as diretrizes de marca da Steinberg, independente da licença do código.

## Outras dependências do JUCE relevantes para o Maestro

Ver a lista completa em [external/JUCE/LICENSE.md](../external/JUCE/LICENSE.md). Nenhuma outra dependência de terceiros do JUCE é usada diretamente pelo Maestro nesta fase (não usamos AAX, ASIO ou AudioUnitSDK no MVP Windows/VST3).
