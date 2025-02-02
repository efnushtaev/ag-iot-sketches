#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

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
}

void loop() {
  // Request temperature readings
  sensors.requestTemperatures();
  // Fetch temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0);
  // Print temperature to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");
  // Wait 1 second before repeating
  delay(1000);
}