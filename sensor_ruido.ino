#include <WiFi.h>
#include <PubSubClient.h>

// ========================================================
// ⚠️ SEGURIDAD: NUNCA COMPARTAS TUS CREDENCIALES REALES
// Reemplaza estos valores con los de tu propia red local
// ========================================================
const char* ssid = "NOME_DA_SUA_REDE_WIFI";       
const char* password = "SENHA_DA_SUA_REDE";  

// ========================================================
// CONFIGURACIÓN MQTT
// ========================================================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
// Usa un tópico único para evitar interferencias con otros usuarios
const char* mqtt_topic = "teu_projeto/sensor/ruido/estudante"; 

WiFiClient espClient;
PubSubClient client(espClient);

// Configuración de Pines ESP32
const int pinEnvelope = 34; // Mide amplitud del sonido
const int pinGate = 18;     // Mide transitorios rápidos

unsigned long ultimoEnvio = 0;
const long intervalo = 1000; // 1 segundo

void setup() {
  Serial.begin(115200);
  
  pinMode(pinEnvelope, INPUT);
  pinMode(pinGate, INPUT);

  Serial.print("Conectando a Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[Wi-Fi] Conectado exitosamente.");

  client.setServer(mqtt_server, mqtt_port);
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32_Device_";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println(" ¡Conectado al Broker!");
    } else {
      Serial.print(" falló, estado: ");
      Serial.print(client.state());
      Serial.println(" Intentando en 5 segundos...");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop();

  unsigned long tiempoActual = millis();
  
  if (tiempoActual - ultimoEnvio >= intervalo) {
    ultimoEnvio = tiempoActual;

    int volumenCrudo = analogRead(pinEnvelope);
    
    // Calibración de sensibilidad para ventanas (0.25)
    float multiplicadorSensibilidad = 0.25; 
    float decibeliosReales = 40.0 + (volumenCrudo * multiplicadorSensibilidad); 
    
    if (decibeliosReales > 100.0) {
      decibeliosReales = 100.0;
    }

    int estadoGate = digitalRead(pinGate);
    String gateString = (estadoGate == HIGH) ? "true" : "false";

    String payload = "{";
    payload += "\"volumen\":" + String(decibeliosReales, 1) + ",";
    payload += "\"gate\":" + gateString;
    payload += "}";

    Serial.print("Enviando -> ");
    Serial.println(payload);

    client.publish(mqtt_topic, payload.c_str());
  }
}
