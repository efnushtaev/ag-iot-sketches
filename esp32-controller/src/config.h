#ifndef CONFIG_H
#define CONFIG_H

// Need add your wifi ssid, password and clientId here
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
#define CLIENT_ID "mqtt-efnushtaev-zue8fh"
#define MQTT_SERVER "dev.rightech.io"
#define MQTT_PORT 1883
#define MQTT_TOPIC_TEMP "base/state/temperature"
#define MQTT_TOPIC_HUMIDITY "base/state/humidity"
#define MQTT_TOPIC_SOIL "base/state/soil"
#define MQTT_TOPIC_RELAY "base/relay/relay1"

#define FC28_ANALOG_PIN 35
#define DHT11_DIGITAL_PIN 4
#define RELAY_DIGITAL_PIN 5

#endif