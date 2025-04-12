// #include <WiFi.h>
// #include <esp_now.h>
// #include <esp_wifi.h>
// #include <DHT.h>

// #define DHTPIN 4
// #define DHTTYPE DHT22

// DHT dht(DHTPIN, DHTTYPE);

// // Структура данных для отправки
// typedef struct struct_message {
//   float temperature;
//   float humidity;
//   unsigned long timestamp;
// } struct_message;

// // MAC-адрес приемника
// uint8_t receiverMac[] = {0x3C, 0xE9, 0x0E, 0x8C, 0xF5, 0xD8}; // Замените на MAC приемника

// constexpr char WIFI_SSID[] = "TP-Link_B354";

// void restart()
// {
//   Serial.println("Restart ESP...");
//   ESP.restart();
// }

// // Callback при отправке данных
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   static int count;
//   if (status == ESP_NOW_SEND_SUCCESS)
//   {
//     Serial.println("Delivery success");
//     count = 0;
//   }
//   else
//   {
//     Serial.print("Delivery fail  ");
//     count++;
//     if (count > 10)
//     {
//       restart();
//     }
//     Serial.println(count);
//   }
// }

// // Глобальные массивы для замеров
// float tempReadings[10];
// float humReadings[10];

// int32_t getWiFiChannel(const char *ssid)
// {
//   if (int32_t n = WiFi.scanNetworks())
//   {
//     for (uint8_t i = 0; i < n; i++)
//     {
//       if (!strcmp(ssid, WiFi.SSID(i).c_str()))
//       {
//         memcpy(&receiverMac, WiFi.BSSID(i), 6);
//         return WiFi.channel(i);
//       }
//     }
//   }
//   return 0;
// }

// bool read_dht_data(float &avgTemp, float &avgHum) {
//   for (int i = 0; i < 10; i++) {
//     float t = dht.readTemperature();
//     float h = dht.readHumidity();

//     if (isnan(t) || isnan(h)) {
//       Serial.println("DHT Read Failed!");
//       return false;
//     }

//     tempReadings[i] = t;
//     humReadings[i] = h;
//     delay(200); // Задержка между измерениями
//   }

//   // Сортировка массивов
//   std::sort(tempReadings, tempReadings + 10);
//   std::sort(humReadings, humReadings + 10);

//   float tempSum = 0, humSum = 0;
//   for (int i = 2; i < 8; i++) { // Используем только центральные 6 значений
//     tempSum += tempReadings[i];
//     humSum += humReadings[i];
//   }

//   avgTemp = tempSum / 6;
//   avgHum = humSum / 6;

//   return true;
// }

// void setup() {
//   Serial.begin(115200);
//   dht.begin();
//   WiFi.mode(WIFI_STA);

//   // Инициализация ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Установка Wi-Fi канала
//   int32_t channel = getWiFiChannel(WIFI_SSID);

//   esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

//   // Регистрация callback
//   esp_now_register_send_cb(OnDataSent);

//   // Добавление пира
//   esp_now_peer_info_t peerInfo = {};
//   memcpy(peerInfo.peer_addr, receiverMac, 6);
//   Serial.println("channel: " + String(channel));
//   peerInfo.channel = channel;
//   peerInfo.encrypt = false;

//   if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//     Serial.println("Failed to add peer");
//     return;
//   }
// }

// void loop() {
//   struct_message myData;
//   myData.timestamp = millis(); // Текущее время в мс

//   // Выполнение замеров с DHT-22
//   if (read_dht_data(myData.temperature, myData.humidity)) {
//     Serial.print("Средняя температура: ");
//     Serial.println(myData.temperature);
//     Serial.print("Средняя влажность: ");
//     Serial.println(myData.humidity);

//     // Отправка данных
//     esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));
//     if (result == ESP_OK) {
//       Serial.println("Данные успешно отправлены");
//     } else {
//       Serial.println("Ошибка отправки данных");
//     }
//   } else {
//     Serial.println("Ошибка чтения данных с DHT-22");
//   }

//   delay(2000); // Отправка каждые 10 секунд
// }