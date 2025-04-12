#ifndef CONFIG_H
#define CONFIG_H

// Need add your wifi ssid, password and clientId here
#define WIFI_SSID "TP-Link_B354"
#define WIFI_PASS "57339016"
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

#endif