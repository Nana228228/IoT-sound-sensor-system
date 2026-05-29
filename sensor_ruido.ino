#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "NOME_DA_SUA_REDE_WIFI";       
const char* password = "SENHA_DA_SUA_REDE";  

// ========================================================
// CONFIGURAÇÃO MQTT
// ========================================================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
// Use um tópico único para evitar interferências com outros usuários
const char* mqtt_topic = "seu_projeto/sensor/ruido/estudante"; 

WiFiClient espClient;
PubSubClient client(espClient);

// Configuração dos Pinos do ESP32
const int pinEnvelope = 34; // Mede a amplitude do som (Volume)
const int pinGate = 18;     // Mede transientes rápidos (Ruídos súbitos)

unsigned long ultimoEnvio = 0;
const long intervalo = 1000; // Enviar dados a cada 1 segundo

void setup() {
  Serial.begin(115200);
  
  pinMode(pinEnvelope, INPUT);
  pinMode(pinGate, INPUT);

  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Wi-Fi] Conectado com sucesso.");

  client.setServer(mqtt_server, mqtt_port);
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32_Device_";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println(" Conectado ao Broker!");
    } else {
      Serial.print(" falhou, estado: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop();

  unsigned long tempoAtual = millis();
  
  if (tempoAtual - ultimoEnvio >= intervalo) {
    ultimoEnvio = tempoAtual;

    // 1. Ler a voltagem bruta real do sensor (0 a 4095)
    int volumeBruto = analogRead(pinEnvelope);
    
    // 2. CONVERSÃO FÍSICA PARA DECIBÉIS (dB)
    // Calibração de sensibilidade para janelas fechadas (0.25)
    float multiplicadorSensibilidade = 0.25; 
    float decibeisReais = 40.0 + (volumeBruto * multiplicadorSensibilidade); 
    
    // Limite físico de segurança para o gráfico
    if (decibeisReais > 100.0) {
      decibeisReais = 100.0;
    }

    // 3. Ler o estado do Gate (Interrupção)
    int estadoGate = digitalRead(pinGate);
    String gateString = (estadoGate == HIGH) ? "true" : "false";

    // 4. Empacotar os dados em formato JSON para o MQTT
    String payload = "{";
    payload += "\"volume\":" + String(decibeisReais, 1) + ",";
    payload += "\"gate\":" + gateString;
    payload += "}";

    // 5. Imprimir no Monitor Serial
    Serial.print("Enviando dados -> ");
    Serial.println(payload);

    // 6. Publicar no Node-RED via MQTT
    client.publish(mqtt_topic, payload.c_str());
  }
}
