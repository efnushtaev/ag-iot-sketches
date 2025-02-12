#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h" // Include the config header file

// Data wire is connected to GPIO 35
#define ONE_WIRE_BUS 35

// MQTT broker details
const char* mqtt_server = "dev.rightech.io";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";
const char* mqtt_topic = "base/state/temperature";

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(CLIENT_ID, mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Start serial communication
  Serial.begin(9600);
  // Start up the library
  sensors.begin();

  DeviceAddress sensorAddress;
  if (!sensors.getAddress(sensorAddress, 0)) {
    Serial.println("No DS18B20 sensor found!");
  } else {
    Serial.print("Found DS18B20 sensor with address: ");
    for (uint8_t i = 0; i < 8; i++) {
      if (sensorAddress[i] < 16) Serial.print("0");
      Serial.print(sensorAddress[i], HEX);
    }
    Serial.println();
  }

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Request temperature readings
  sensors.requestTemperatures();
  // Fetch temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0);
  // Check if the reading is valid
  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
  } else {
    // Print temperature to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    // Publish temperature to MQTT
    char tempString[8];
    dtostrf(temperatureC, 1, 2, tempString);
    client.publish(mqtt_topic, tempString);
  }
  // Wait 1 second before repeating
  delay(10000);
}