// ----------------------------------------------------------------------------
// --- ESP32 GATEWAY ESP-NOW + MQTT
// ----------------------------------------------------------------------------

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <PubSubClient.h>
#include "config.h"

TaskHandle_t espNowTaskHandle;
TaskHandle_t mqttTaskHandle;

// ----------------------------------------------------------------------------
// WiFi
// ----------------------------------------------------------------------------

void initWiFi()
{
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.begin(WIFI_SSID_2_4, WIFI_PASSWORD);

  Serial.printf("Connecting to %s .", WIFI_SSID_2_4);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println(" ok");

  IPAddress ip = WiFi.localIP();

  Serial.printf("SSID: %s\n", WIFI_SSID_2_4);
  Serial.printf("Channel: %u\n", WiFi.channel());
  Serial.printf("IP: %u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
}

// ----------------------------------------------------------------------------
// ESP-NOW
// ----------------------------------------------------------------------------

typedef struct struct_message
{
  float temperature;
  float humidity;
  float soil;
  unsigned long timestamp;
} struct_message;

struct_message myData;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  Serial.print("Получено данных от: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.printf("%02X", mac[i]);
    if (i < 5)
      Serial.print(":");
  }
  Serial.println();

  if (len == sizeof(myData))
  {
    memcpy(&myData, incomingData, sizeof(myData));

    Serial.print("Получено данных: ");
    Serial.println(len);
    Serial.print("TIME: ");
    Serial.println(myData.timestamp);
    Serial.print("TEMP: ");
    Serial.println(myData.temperature);
    Serial.print("HUMI: ");
    Serial.println(myData.humidity);
    Serial.println();
  }
  else
  {
    Serial.println("Ошибка: полученные данные имеют неправильный размер");
  }
}

void espNowTask(void *pvParameters)
{
  while (true)
  {
    // Основной цикл не требуется, данные обрабатываются в callback
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void initEspNow()
{
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP NOW failed to initialize");
    while (1)
      ;
  }

  esp_now_register_recv_cb(onDataRecv);
  xTaskCreatePinnedToCore(espNowTask, "ESP-NOW Task", 4096, NULL, 1, &espNowTaskHandle, 0);
}

// ----------------------------------------------------------------------------
// MQTT
// ----------------------------------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_topic_time = MQTT_TOPIC_STATUS_ESP;
const char *mqtt_topic_temp = MQTT_TOPIC_TEMP;
const char *mqtt_topic_humi = MQTT_TOPIC_HUMIDITY;
const char *mqtt_client = CLIENT_ID;

enum PublishType
{
  TEMP,
  HUMI,
  SOIL,
  BATT,
  TIME
};

PublishType nextPublish = TIME;

const unsigned long interval = 20000; // Интервал публикации в миллисекундах
unsigned long lastMillisPublish = 0;

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_client))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttTask(void *pvParameters)
{
  while (true)
  {
    if (!client.connected())
    {
      reconnect();
    }

    client.loop();

    unsigned long currentMillis = millis();

    // --------------------------------------------------------------------------------------------
    // TIME
    // --------------------------------------------------------------------------------------------

    if (currentMillis - lastMillisPublish >= interval && nextPublish == TIME)
    {
      lastMillisPublish = currentMillis;

      char msg[50];
      snprintf(msg, sizeof(msg), "%lu", myData.timestamp);
      client.publish(mqtt_topic_time, msg);

      Serial.print("Timestamp to MQTT: ");
      Serial.println(msg);
      Serial.println('\n');

      nextPublish = TEMP;
    };

    // --------------------------------------------------------------------------------------------
    // TEMP
    // --------------------------------------------------------------------------------------------

    if (currentMillis - lastMillisPublish >= interval && nextPublish == TEMP)
    {
      lastMillisPublish = currentMillis;

      char msg[50];
      snprintf(msg, sizeof(msg), "%.2f", myData.temperature);
      client.publish(mqtt_topic_temp, msg);

      Serial.print("Temperature to MQTT: ");
      Serial.println(msg);
      Serial.println('\n');

      nextPublish = HUMI;
    };

    // --------------------------------------------------------------------------------------------
    // HUMI
    // --------------------------------------------------------------------------------------------

    if (currentMillis - lastMillisPublish >= interval && nextPublish == HUMI)
    {
      lastMillisPublish = currentMillis;

      char msg[50];
      snprintf(msg, sizeof(msg), "%.2f", myData.humidity);
      client.publish(mqtt_topic_humi, msg);

      Serial.print("Humidity to MQTT: ");
      Serial.println(msg);
      Serial.println('\n');

      nextPublish = SOIL;
    };

    // --------------------------------------------------------------------------------------------
    // SOIL
    // --------------------------------------------------------------------------------------------

    if (currentMillis - lastMillisPublish >= interval && nextPublish == SOIL)
    {
      lastMillisPublish = currentMillis;

      char msg[50];
      snprintf(msg, sizeof(msg), "%.0f%%", myData.soil); // Округление до ближайшего целого
      client.publish(mqtt_topic_humi, msg);

      Serial.print("Soil to MQTT: ");
      Serial.println(msg);
      Serial.println('\n');

      nextPublish = TIME;
    };

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void initMqtt()
{
  client.setServer(mqtt_server, mqtt_port);
  xTaskCreatePinnedToCore(mqttTask, "MQTT Task", 4096, NULL, 1, &mqttTaskHandle, 1);
}

// ----------------------------------------------------------------------------
// INIT + LOOP
// ----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(500);

  initWiFi();
  initEspNow();
  initMqtt();
}

void loop() {}
