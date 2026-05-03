// TRANSMISSOR - LoRa + DHT11 + SD + GPS
#include <SPI.h>
#include <SD.h>
#include <LoRa.h>
#include <Wire.h>
#include "DHT.h"
#include "SSD1306Wire.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define SD_CS    17
#define SD_SCK   22
#define SD_MOSI  23
#define SD_MISO  21
#define DHTPIN   25

#define LORA_SS    18
#define LORA_RST   14
#define LORA_DIO0  26

#define GPS_RX   13
#define GPS_TX   12

#define DISPLAY_TIMEOUT_MS  60000UL   // display fica ligado 1 minuto
#define INTERVALO_ENVIO_MS  20000UL   // medição a cada 20 s

SSD1306Wire display(0x3c, 4, 15);
DHT dht(DHTPIN, DHT11);
SPIClass spiSD(HSPI);
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

bool displayAtivo = true;

void desligarDisplay() {
  display.displayOff();
  displayAtivo = false;
  Serial.println("Display apagado para economizar bateria.");
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW); delay(50); digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 0, "Iniciando TX...");
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

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("GPS iniciado");

  dht.begin();
  delay(2000);
}

void loop() {
  // Apaga o display após 1 minuto
  if (displayAtivo && millis() > DISPLAY_TIMEOUT_MS) {
    desligarDisplay();
  }

  // Alimenta o GPS em background continuamente
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > INTERVALO_ENVIO_MS) {
    ultimoEnvio = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      float t_cal = 27.6940 + (-0.0525 * t);
      float h_cal = 46.5297 + (0.2679 * h);

      bool gpsOk = gps.location.isValid() && gps.location.age() < 5000;
      float lat = gpsOk ? gps.location.lat() : 0.0;
      float lng = gpsOk ? gps.location.lng() : 0.0;

      Serial.println("=== TX ===");
      Serial.print("T corrigida: "); Serial.print(t_cal); Serial.println(" C");
      Serial.print("U corrigida: "); Serial.print(h_cal); Serial.println(" %");
      Serial.print("GPS Sats: "); Serial.println(gps.satellites.value());
      if (gpsOk) {
        Serial.print("LAT: "); Serial.println(lat, 6);
        Serial.print("LNG: "); Serial.println(lng, 6);
      } else {
        Serial.println("GPS: sem sinal");
      }

      String dataString = String(t_cal, 1) + "," + String(h_cal, 1) + ","
                        + String(lat, 6) + "," + String(lng, 6);

      LoRa.beginPacket();
      LoRa.print(dataString);
      LoRa.endPacket();

      File file = SD.open("/datalog.txt", FILE_APPEND);
      if (file) {
        file.println(dataString);
        file.close();
        Serial.println("SD: gravado");
      } else {
        Serial.println("SD: erro ao gravar");
      }

      if (displayAtivo) {
        display.clear();
        display.drawString(0, 0,  "[TX] Enviando...");
        display.drawString(0, 15, "T: " + String(t_cal, 1) + " C");
        display.drawString(0, 28, "U: " + String(h_cal, 1) + " %");
        if (gpsOk) {
          display.drawString(0, 41, "LAT: " + String(lat, 4));
          display.drawString(0, 54, "LNG: " + String(lng, 4));
        } else {
          display.drawString(0, 41, "GPS: buscando...");
          display.drawString(0, 54, "Sats: " + String(gps.satellites.value()));
        }
        display.display();
      }

      Serial.println("Enviado: " + dataString);

    } else {
      Serial.println("Erro na leitura do DHT!");
      if (displayAtivo) {
        display.clear();
        display.drawString(0, 0, "Erro no DHT11!");
        display.display();
      }
    }
  }
}