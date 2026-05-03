// Código utilizado para a CALIBRAÇÃO DO SENSOR DE TEMPERATURA

#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <Wire.h>               
#include "SSD1306Wire.h" // Biblioteca para o display

#define SD_CS 5
#define DHTPIN 25
#define DHTTYPE DHT11

// Configuração do display para os pinos padrão 
SSD1306Wire display(0x3c, 4, 15); 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Inicializa o display 
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW); 
  delay(50); 
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Iniciando...");
  display.display();

  Serial.println("Iniciando sistema...");
  dht.begin();

  // Lógica cartão SD
  SPI.begin(18, 19, 23);

  Serial.println("Inicializando SD...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERRO: Falha ao montar o cartão SD");
    display.drawString(0, 15, "Erro SD!");
    display.display();
    return;
  }
  Serial.println("SD OK");
  display.drawString(0, 15, "SD OK!");
  display.display();

  if (!SD.exists("/datalog.txt")) {
    File file = SD.open("/datalog.txt", FILE_WRITE);
    if (file) {
      file.println("Temperatura (C), Umidade (%)");
      file.close();
    }
  }
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("Erro leitura DHT");
    display.clear();
    display.drawString(0, 0, "Erro DHT");
    display.display();
    delay(2000);
    return;
  }

  // Exibição no Display
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Temp: " + String(t) + "C");
  display.drawString(0, 25, "Umid: " + String(h) + "%");
  display.display();

  // Log no Terminal e no SD 
  String dataString = String(t) + ", " + String(h);
  Serial.println("Gravando: " + dataString);

  File file = SD.open("/datalog.txt", FILE_APPEND);
  if (file) {
    file.println(dataString);
    file.close();
    Serial.println("Sucesso.");
  } else {
    Serial.println("Erro ao abrir datalog.txt");
  }

  delay(10000); // Intervalo de 30 segundos
}