// #include <WiFi.h>
// #include <esp_now.h>
// #include <esp_wifi.h>
// #include <PubSubClient.h>
// #include "config.h"

// #define WIFI_CHANNEL 1 // Установите нужный канал Wi-Fi

// // Настройки MQTT
// const char *mqtt_server = MQTT_SERVER;                 // Замените на адрес вашего MQTT сервера
// const int mqtt_port = 1883;                            // Порт MQTT сервера
// const char *mqtt_topic = MQTT_TOPIC_STATUS_ESP;        // Топик для отправки временной метки
// const char *mqtt_topic_humidity = MQTT_TOPIC_HUMIDITY; // Топик для отправки временной метки
// const char *mqtt_topic_temperature = MQTT_TOPIC_TEMP;  // Топик для отправки временной метки

// enum PublishType
// {
//   TEMP,
//   HUMI,
//   SOIL,
//   BATT,
//   TIME
// };
// PublishType nextPublish = TIME;

// unsigned long lastMillisPublish = 0;
// const unsigned long interval = 20000;

// WiFiClient espClient;
// PubSubClient client(espClient);

// // Структура данных (должна совпадать на передатчике и приемнике)
// typedef struct __attribute__((packed)) struct_message
// {
//   unsigned long timestamp;
// } struct_message;

// struct_message myData;

// TaskHandle_t espNowTaskHandle;
// TaskHandle_t mqttTaskHandle;

// void setup_wifi()
// {
//   delay(10);
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(WIFI_SSID); // Замените на ваш SSID

//   WiFi.mode(WIFI_MODE_APSTA);           // Режим станции
//   esp_wifi_set_ps(WIFI_PS_NONE); // NO POWER SAVE Важно для стабильности!
//   WiFi.setAutoConnect(true);
//   WiFi.setAutoReconnect(true);
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Замените на ваш SSID и пароль

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// // Callback-функция при получении данных
// void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
// {
//   Serial.print("Получено данных от: ");
//   for (int i = 0; i < 6; i++)
//   {
//     Serial.printf("%02X", mac[i]);
//     if (i < 5)
//       Serial.print(":");
//   }
//   Serial.println();

//   if (len == sizeof(myData))
//   {
//     memcpy(&myData, incomingData, sizeof(myData));

//     Serial.print("Получено данных: ");
//     Serial.println(len);
//     Serial.print("Timestamp: ");
//     Serial.println(myData.timestamp);
//     Serial.println();
//   }
//   else
//   {
//     Serial.println("Ошибка: полученные данные имеют неправильный размер");
//   }
// }

// // void reconnect()
// // {
// //   // Loop until we're reconnected
// //   while (!client.connected())
// //   {
// //     Serial.print("Attempting MQTT connection...");
// //     // Attempt to connect
// //     if (client.connect(CLIENT_ID))
// //     {
// //       Serial.println("connected");
// //     }
// //     else
// //     {
// //       Serial.print("failed, rc=");
// //       Serial.print(client.state());
// //       Serial.println(" try again in 5 seconds");
// //       // Wait 5 seconds before retrying
// //       delay(5000);
// //     }
// //   }
// // }


// // void mqttTask(void *pvParameters)
// // {
// //   while (true)
// //   {
// //     if (!client.connected())
// //     {
// //       reconnect();
// //     }

// //     client.loop();

// //     unsigned long currentMillis = millis();

// //     if (currentMillis - lastMillisPublish >= interval && nextPublish == TIME)
// //     {
// //       lastMillisPublish = currentMillis;

// //       // Публикация временной метки в MQTT
// //       char msg[50];
// //       snprintf(msg, sizeof(msg), "%lu", myData.timestamp);
// //       client.publish(mqtt_topic, msg);

// //       Serial.print("Timestamp to MQTT: ");
// //       Serial.println(msg);
// //       Serial.println('\n');
// //     };

// //     vTaskDelay(100 / portTICK_PERIOD_MS);
// //   }
// // }


// void espNowTask(void *pvParameters)
// {
//   while (true)
//   {
//     // Основной цикл не требуется, данные обрабатываются в callback
//     vTaskDelay(200 / portTICK_PERIOD_MS);
//   }
// }

// void setup()
// {
//   Serial.begin(115200);
//   Serial.print("MAC Address: ");
//   Serial.println(WiFi.macAddress());

//   setup_wifi();
//   client.setServer(mqtt_server, mqtt_port);

//   // Инициализация ESP-NOW
//   if (esp_now_init() != ESP_OK)
//   {
//     Serial.println("Ошибка инициализации ESP-NOW");
//     return;
//   }

//   Serial.println("ESP-NOW инициализирован");

//   // Регистрация callback для приема данных
//   esp_now_register_recv_cb(OnDataRecv);
//   Serial.println("Callback для приема данных зарегистрирован");

//   // Создание задач для работы с ESP-NOW и MQTT
//   xTaskCreatePinnedToCore(espNowTask, "ESP-NOW Task", 4096, NULL, 1, &espNowTaskHandle, 0);
//   // xTaskCreatePinnedToCore(mqttTask, "MQTT Task", 4096, NULL, 1, &mqttTaskHandle, 1);
// }

// void loop()
// {
//   //
// }