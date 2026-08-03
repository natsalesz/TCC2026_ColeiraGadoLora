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

Este projeto consiste no desenvolvimento de um protótipo de coleira inteligente para monitoramento ambiental e geolocalização de bovinos. Utilizando o protocolo **LoRa**, o sistema transmite dados de temperatura, umidade e coordenadas GPS em tempo real, visando o cálculo do Índice de Temperatura e Umidade (THI) para avaliação do conforto térmico animal.

---

## Organização do Repositório

### Codigos_Arduino/

Contém os códigos de transmissão (TX) e recepção (RX) utilizados no protótipo final.

Antes de carregar os códigos, instale as seguintes bibliotecas na Arduino IDE:

- `DHT sensor library` — Adafruit
- `LoRa` — Sandeep Mistry
- `TinyGPSPlus` — Mikal Hart

### Calibracao_DHT11/

Contém o código Arduino utilizado na etapa de calibração dos sensores DHT11, bem como o notebook Python (Google Colaboratory) com a análise estatística realizada para obtenção das equações de correção de temperatura e umidade.

Antes de carregar o código de calibração, instale a seguinte biblioteca na Arduino IDE:

- `SSD1306Wire` — ThingPulse

---

## 🛠 Tecnologias Utilizadas

### Hardware

- ESP32 Heltec LoRa V2
- Módulo LoRa SX1276 (868 MHz)
- Sensor DHT11
- Módulo GPS NEO-6M
- Módulo MicroSD

### Software

- Arduino IDE
- Linguagem C++
- Python
- Google Colaboratory

### Bibliotecas

- DHT Sensor Library (Adafruit)
- LoRa (Sandeep Mistry)
- TinyGPSPlus (Mikal Hart)
- SSD1306Wire (ThingPulse)

---

## 📊 Resultados

O protótipo desenvolvido foi capaz de:

- Coletar dados de temperatura e umidade ambiente utilizando o sensor DHT11;
- Obter a localização geográfica por meio do módulo GPS NEO-6M;
- Transmitir os dados entre os dispositivos utilizando comunicação LoRa;
- Armazenar os dados coletados em cartão MicroSD;
- Calcular o Índice de Temperatura e Umidade (THI), utilizado para avaliação do conforto térmico animal;
- Validar experimentalmente o funcionamento do sistema por meio de testes de comunicação, calibração dos sensores e análise estatística dos dados coletados.

---

## 📄 Documentação

A documentação completa do projeto está disponível no Trabalho de Conclusão de Curso (TCC), contendo:

- Introdução e fundamentação teórica;
- Objetivos geral e específicos;
- Materiais utilizados no desenvolvimento do protótipo;
- Metodologia de implementação;
- Desenvolvimento do sistema embarcado;
- Calibração dos sensores DHT11;
- Testes de comunicação utilizando LoRa;
- Coleta e armazenamento dos dados;
- Análise estatística dos resultados;
- Discussão dos resultados obtidos;
- Conclusões.

O documento completo pode ser consultado no arquivo:
📄 **`Documentacao/TCC_Coleira_Inteligente_LoRa.pdf`**

---

## Licença

Este projeto foi desenvolvido para fins acadêmicos como Trabalho de Conclusão de Curso do curso de Tecnologia em Análise e Desenvolvimento de Sistemas do Instituto Federal de Educação, Ciência e Tecnologia de São Paulo (IFSP) – Campus Campinas.
