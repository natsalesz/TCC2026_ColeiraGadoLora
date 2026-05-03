// RECEPTOR - LoRa + DHT11 + SD + GPS
#include <SPI.h>
#include <SD.h>
#include <LoRa.h>
#include <Wire.h>
#include "DHT.h"
#include "SSD1306Wire.h"

#define SD_CS    17
#define SD_SCK   22
#define SD_MOSI  23
#define SD_MISO  21
#define DHTPIN   25

#define LORA_SS    18
#define LORA_RST   14
#define LORA_DIO0  26

#define DISPLAY_TIMEOUT_MS  60000UL    // display ligado por 1 minuto
#define INTERVALO_LOCAL_MS  20000UL   // leitura local a cada 20 s

SSD1306Wire display(0x3c, 4, 15);
DHT dht(DHTPIN, DHT11);
SPIClass spiSD(HSPI);

bool displayAtivo = true;
unsigned long ultimaLeituraLocal = 0;

void desligarDisplay() {
  display.clear();
  display.display();
  display.setContrast(0);
  display.displayOff();
  displayAtivo = false;
  Serial.println("Display apagado para economizar bateria.");
}

void gravarSD(const char* arquivo, String dados) {
  File file = SD.open(arquivo, FILE_APPEND);
  if (file) {
    file.println(dados);
    file.close();
    Serial.println("SD gravado em " + String(arquivo) + ": " + dados);
  } else {
    Serial.println("Erro ao gravar em " + String(arquivo));
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 0, "Iniciando RX...");
  display.display();

  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);  delay(200);
  digitalWrite(LORA_RST, HIGH); delay(200);

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  LoRa.setSyncWord(0xF3);
  if (!LoRa.begin(915E6)) {
    display.drawString(0, 15, "LORA: ERRO!");
    display.display();
    Serial.println("LoRa falhou!");
    while (true);
  }
  Serial.println("LoRa OK");
  display.drawString(0, 15, "LORA: OK");
  display.display();

  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD)) {
    display.drawString(0, 30, "SD: ERRO!");
    Serial.println("SD falhou!");
  } else {
    display.drawString(0, 30, "SD: OK!");
    Serial.println("SD OK");
  }
  display.display();

  dht.begin();
  delay(2000);
}

void loop() {
  // Apaga o display após 3 minutos
  if (displayAtivo && millis() > DISPLAY_TIMEOUT_MS) {
    desligarDisplay();
  }

  // Leitura local DHT11 a cada 20 s — só SD e Serial, sem display
  unsigned long agora = millis();
  if (agora - ultimaLeituraLocal >= INTERVALO_LOCAL_MS) {
    ultimaLeituraLocal = agora;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      float t_cal = 27.6940 + (-0.0525 * t);
      float h_cal = 46.5297 + (0.2679 * h);

      Serial.println("=== RX Local ===");
      Serial.print("T bruta: "); Serial.print(t); Serial.println(" C");
      Serial.print("T corrigida: "); Serial.print(t_cal); Serial.println(" C");
      Serial.print("U bruta: "); Serial.print(h); Serial.println(" %");
      Serial.print("U corrigida: "); Serial.print(h_cal); Serial.println(" %");

      String dadosLocais = String(t_cal, 1) + "," + String(h_cal, 1);
      gravarSD("/local.txt", dadosLocais);

    } else {
      Serial.println("Erro na leitura DHT11 local!");
    }
  }

  // Recepção LoRa
  if (LoRa.parsePacket()) {
    String dadosRecebidos = "";
    while (LoRa.available()) {
      dadosRecebidos += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    Serial.println("Recebido: " + dadosRecebidos);
    Serial.println("RSSI: " + String(rssi) + " dBm");

    gravarSD("/recebido.txt", dadosRecebidos);

    // Parse dos 4 campos: temp,umid,lat,lng
    int sep1 = dadosRecebidos.indexOf(',');
    int sep2 = dadosRecebidos.indexOf(',', sep1 + 1);
    int sep3 = dadosRecebidos.indexOf(',', sep2 + 1);

    if (sep1 != -1 && sep2 != -1 && sep3 != -1) {
      String sTemp = dadosRecebidos.substring(0, sep1);
      String sUmid = dadosRecebidos.substring(sep1 + 1, sep2);
      String sLat  = dadosRecebidos.substring(sep2 + 1, sep3);
      String sLng  = dadosRecebidos.substring(sep3 + 1);

      Serial.println("T: " + sTemp + " C");
      Serial.println("U: " + sUmid + " %");
      Serial.println("LAT: " + sLat);
      Serial.println("LNG: " + sLng);

      if (displayAtivo) {
        display.clear();
        display.drawString(0, 0,  "[RX] Recebido:");
        display.drawString(0, 13, "T: " + sTemp + " C  U: " + sUmid + " %");
        display.drawString(0, 26, "LAT: " + sLat);
        display.drawString(0, 39, "LNG: " + sLng);
        display.drawString(0, 52, "RSSI: " + String(rssi) + " dBm");
        display.display();
      }

    } else {
      // Formato sem GPS (retrocompatível)
      if (sep1 != -1) {
        String sTemp = dadosRecebidos.substring(0, sep1);
        String sUmid = dadosRecebidos.substring(sep1 + 1);

        if (displayAtivo) {
          display.clear();
          display.drawString(0, 0,  "[RX] Recebido:");
          display.drawString(0, 15, "T: " + sTemp + " C");
          display.drawString(0, 28, "U: " + sUmid + " %");
          display.drawString(0, 41, "RSSI: " + String(rssi) + " dBm");
          display.display();
        }
      }
    }
  }
}