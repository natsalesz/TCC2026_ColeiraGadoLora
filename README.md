# TCC2026_ColeiraGadoLora

## Autores

| Nome | RA |
|------|--------|
| Natasha Sales Ferreira Pinto | CP3025799 |
| Pedro Enrico da Silva Serradilha | CP3025659 |

---

**Coleira de Rastreamento para Gado com Monitoramento em Espaços Agropecuários** 

Instituto Federal de São Paulo — Campus Campinas  
Curso: Tecnologia em Análise e Desenvolvimento de Sistemas  
Orientador: Prof. Dr. Glauber da Rocha Balthazar  

---

## Sobre o Projeto

Este projeto consiste no desenvolvimento de um protótipo de coleira inteligente (wearable animal) para monitoramento ambiental e geolocalização de bovinos. Utilizando o protocolo **LoRa**, o sistema transmite dados de temperatura, umidade e coordenadas GPS em tempo real, visando o cálculo do Índice de Temperatura e Umidade (THI) para avaliação do conforto térmico animal.

---

## Organização do Repositório

**Codigos_Arduino/** — Contém os códigos de transmissão (TX) e recepção (RX) utilizados no protótipo final. Antes de carregar os códigos, instale as seguintes bibliotecas na Arduino IDE:
- `DHT sensor library` — Adafruit
- `LoRa` — Sandeep Mistry
- `TinyGPSPlus` — Mikal Hart

**Calibracao_Arduino/** — Contém o código Arduino utilizado na etapa de calibração dos sensores DHT11, bem como o notebook Python (Google Colaboratory) com a análise estatística realizada para obtenção das equações de correção de temperatura e umidade. Antes de carregar o código de calibração, instale a seguinte biblioteca na Arduino IDE:
- `SSD1306Wire` — ThingPulse
