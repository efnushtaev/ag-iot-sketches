#ifndef CONFIG_H
#define CONFIG_H

// Need add your wifi ssid, password and clientId here
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASS "WIFI_PASS"
#define CLIENT_ID "mqtt-efnushtaev-zue8fh"
#define MQTT_SERVER "dev.rightech.io"
#define MQTT_PORT 1883

// MQTT topics
#define MQTT_TOPIC_TEMP "base/state/temperature"
#define MQTT_TOPIC_HUMIDITY "base/state/humidity"
#define MQTT_TOPIC_SOIL "base/state/soil"
#define MQTT_TOPIC_BATTERY "base/state/battery"
#define MQTT_TOPIC_RELAY_WATER "base/relay/water"
#define MQTT_TOPIC_RELAY_LIGHT "base/relay/light"
#define MQTT_TOPIC_RELAY_FAN "base/relay/fan"
#define MQTT_TOPIC_STATUS_ESP "base/state/statusEsp"

#define FC28_ANALOG_PIN 35
#define DHT11_DIGITAL_PIN 19
#define RELAY_DIGITAL_PIN 5

#define BATTERY_A_PIN 34
#define SOIL_A_PIN 35
#define DHT22_D_PIN 4

#define DHT_INTERVAL_TIMEOUT 5000
#define SOIL_INTERVAL_TIMEOUT 30000

#endif