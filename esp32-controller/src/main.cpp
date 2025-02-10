#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define SOIL_MOISTURE_PIN A0
#define DHTPIN 4
#define DHTTYPE DHT22
#define RELAY_PIN 5

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "your_mqtt_server";
const int mqtt_port = 1883;
const char* mqtt_topic_temp = "sensor/temperature";
const char* mqtt_topic_humidity = "sensor/humidity";
const char* mqtt_topic_soil = "sensor/soil_moisture";
const char* mqtt_topic_relay = "relay/control";

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  if (String(topic) == mqtt_topic_relay) {
    if (message == "ON") {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Relay is ON");
    } else if (message == "OFF") {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Relay is OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_relay);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity from DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature and humidity to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Publish temperature and humidity to MQTT
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  client.publish(mqtt_topic_temp, tempString);

  char humidityString[8];
  dtostrf(humidity, 1, 2, humidityString);
  client.publish(mqtt_topic_humidity, humidityString);

  // Read soil moisture
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);

  // Publish soil moisture to MQTT
  char soilString[8];
  itoa(soilMoistureValue, soilString, 10);
  client.publish(mqtt_topic_soil, soilString);

  // Wait 10 seconds before repeating
  delay(10000);
}