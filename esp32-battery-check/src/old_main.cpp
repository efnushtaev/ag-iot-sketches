// #include <WiFi.h>
// #include <esp_now.h>
// #include <DHT.h>

// #define DHTPIN 4 // Пин, к которому подключен DHT22
// #define DHTTYPE DHT22

// DHT dht(DHTPIN, DHTTYPE);

// const int BATTERY_PIN = 34; // Пин для измерения напряжения на аккумуляторе

// const float R1 = 100000.0;        // Резистор R1 (100k)
// const float R2 = 220000.0;        // Резистор R2 (220k)
// const float VREF = 3.3;           // Опорное напряжение
// const float CALIBRATION = 1.1;    // Калибровочный коэффициент
// #define SAMPLES 10       // Количество образцов
// #define SAMPLE_DELAY 20  // Задержка между замерами (мс)
// #define SLEEP_MINUTES 1  // Интервал сна

// // Замените на MAC-адрес вашего приемника
// uint8_t receiverMac[] = {0x3C, 0xE9, 0x0E, 0x8C, 0xF5, 0xD8};

// // Структура данных (должна совпадать на передатчике и приемнике)
// typedef struct __attribute__((packed)) struct_message {
//     float temperature;
//     float humidity;
//     float battery;
// } struct_message;

// struct_message myData;

// // Callback-функция после отправки данных
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//     Serial.print("Статус отправки: ");
//     Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Успешно" : "Ошибка");
//     if (status != ESP_NOW_SEND_SUCCESS) {
//         Serial.print("Ошибка отправки: ");
//         Serial.println(status);
//     }
// }

// // Функция для медианной фильтрации
// float read_filtered_voltage() {
//     int samples[SAMPLES];

//     // Сбор образцов
//     for(int i=0; i<SAMPLES; i++) {
//         samples[i] = analogRead(BATTERY_PIN);
//         delay(SAMPLE_DELAY);
//     }
  
//     // Сортировка и усреднение центральных значений
//     std::sort(samples, samples+SAMPLES);
//     long sum = 0;
//     for(int i=2; i<SAMPLES-2; i++) { // Отбрасываем 2 мин и 2 макс
//         sum += samples[i];
//     }

//     return (sum * VREF) / ((SAMPLES-4) * 4095.0); // Изменено на 4095 для ESP32
// }

// void setup() {
//     Serial.begin(115200);
//     Serial.println("Инициализация датчика DHT...");
//     dht.begin();
//     Serial.println("Инициализация завершена.");
//     WiFi.mode(WIFI_STA); // Режим станции
    
//     // Инициализация ESP-NOW
//     if (esp_now_init() != ESP_OK) {
//         Serial.println("Ошибка инициализации ESP-NOW");
//         return;
//     }
//     Serial.println("ESP-NOW инициализирован");
    
//     // Регистрация callback для отслеживания статуса отправки
//     esp_now_register_send_cb(OnDataSent