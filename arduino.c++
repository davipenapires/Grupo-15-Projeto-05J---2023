#include <DHTesp.h>
#include <EspMQTTClient.h>

#define SSID_NAME "Wokwi-GUEST"
#define SSID_PASS ""
#define DHT_PIN 15
#define BROKER_URL "broker.hivemq.com"
#define BROKER_USERNAME ""
#define BROKER_PASSWORD ""
#define MQTT_CLIENT_NAME "leticia-mack"
#define BROKER_PORT 1883
#define TOPICO_PREFIXO "medirtemperatura"
#define TOPICO_TEMPERATURA TOPICO_PREFIXO"/Temperatura"
#define TOPICO_UMIDADE TOPICO_PREFIXO"/Umidade"

DHTesp dhtSensor;
EspMQTTClient clienteMQTT(SSID_NAME, SSID_PASS, BROKER_URL, BROKER_USERNAME, BROKER_PASSWORD, MQTT_CLIENT_NAME, BROKER_PORT);

void onConnectionEstablished() {}

void enviarDados() {
  TempAndHumidity temp_umid = dhtSensor.getTempAndHumidity();
  clienteMQTT.publish(TOPICO_TEMPERATURA, String(temp_umid.temperature, 2) + "°C"); 
  clienteMQTT.publish(TOPICO_UMIDADE, String(temp_umid.humidity, 1) + "%");
}

void statusConnection() {
  if (clienteMQTT.isWifiConnected()) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected()) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName()) +
  " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp()) +
  " / Porta: " + String(clienteMQTT.getMqttServerPort()));
}

void setup() {
  Serial.begin(9600);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  clienteMQTT.enableDebuggingMessages();
}

void loop() {
  clienteMQTT.loop();
  enviarDados();
  statusConnection();
  delay(5000);
}