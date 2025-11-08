// Sistema GPS IoT - Sprint 2 SIMPLIFICADO
// ESP32 → MQTT → Node-RED → Oracle DB

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "fiap/gps/dados";
const char* mqtt_client_id = "ESP32_GPS_FIAP";

// Objetos
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Variáveis GPS
float latitude = -23.550520;
float longitude = -46.633308;
float velocidade = 40.0;
int contador = 0;
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n================================");
  Serial.println("   GPS IoT - Sprint 2 FIAP");
  Serial.println("================================\n");
  
  conectarWiFi();
  mqtt.setServer(mqtt_server, mqtt_port);
  
  Serial.println("✓ Sistema iniciado!\n");
}

void loop() {
  if (!mqtt.connected()) {
    reconectarMQTT();
  }
  mqtt.loop();
  
  if (millis() - ultimoEnvio > 5000) {
    ultimoEnvio = millis();
    contador++;
    enviarDadosMQTT();
  }
}

void conectarWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" ✓");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void reconectarMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    
    if (mqtt.connect(mqtt_client_id)) {
      Serial.println(" ✓");
    } else {
      Serial.print(" ✗ (Erro ");
      Serial.print(mqtt.state());
      Serial.println(")");
      delay(3000);
    }
  }
}

void enviarDadosMQTT() {
  StaticJsonDocument<200> doc;
  
  doc["dispositivo"] = mqtt_client_id;
  doc["latitude"] = latitude;
  doc["longitude"] = longitude;
  doc["velocidade"] = velocidade;
  doc["contador"] = contador;
  doc["timestamp"] = millis() / 1000;
  
  char buffer[200];
  serializeJson(doc, buffer);
  
  if (mqtt.publish(mqtt_topic, buffer)) {
    Serial.println("✓ Enviado via MQTT");
    Serial.print("  Tópico: ");
    Serial.println(mqtt_topic);
    Serial.print("  Dados: ");
    Serial.println(buffer);
  } else {
    Serial.println("✗ Erro ao enviar");
  }
  
  Serial.println("─────────────────────\n");
}