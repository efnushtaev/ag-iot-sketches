# atsap-iot-sketches

*Скетчи для IoT девайсов*

Платформа - **ESP32 DOIT DevKit V1**
Фреймворк - **Arduino**

## 👇 Что внутри?

#### [_esp32-gateway-template_](./esp32-gateway-template/)

Шаблон приложения для работы ESP32 в качестве шлюза (**gateway**) в сети ESP-NOW

#### [_esp32-node-template_](./esp32-node-template/)

Шаблон приложения для работы ESP32 в качестве узла (**node**) в сети ESP-NOW 

#### [_esp32-controller_](./esp32-controller/)

Приложение для работы ESP32 в качестве шлюза (**gateway**) в сети ESP-NOW. Собирает данные от узлов в ESP-NOW, и отправляет в MQTT-брокер по WiFi

#### [_esp32-node-dht22_](./esp32-node-dht22/)

Приложение для работы ESP32 в качестве узла (**node**) в сети ESP-NOW. Cобирает данные с датчика температуры и влажности DHT-22


## 🛠 Рецепты для скетчей

### Получение доступного WiFi канала

```
int32_t getWiFiChannel(const char *ssid)
{
    if (int32_t n = WiFi.scanNetworks())
    {
        for (uint8_t i = 0; i < n; i++)
        {
            if (!strcmp(ssid, WiFi.SSID(i).c_str()))
            {
                return WiFi.channel(i);
            }
        }
    }

    return 0;
}
```

#### 🔎 Как это работает?

Функция `getWiFiChannel` определяет канал Wi-Fi сети по её SSID. Она выполняет сканирование доступных сетей с помощью `WiFi.scanNetworks()` и сравнивает SSID каждой найденной сети с переданным значением.

Если совпадение найдено, возвращается канал этой сети через `WiFi.channel(i)`. Если сеть не найдена или сканирование не удалось, функция возвращает `0`.

### Получение данных с датчика DHT-22

```
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

typedef struct struct_dht
{
    float temperature;
    float humidity;
} struct_dht;

struct_dht myData;

float tempReadings[10];
float humReadings[10];

bool read_dht_data(float &avgTemp, float &avgHum)
{
    for (int i = 0; i < 10; i++)
    {
        float t = dht.readTemperature();
        float h = dht.readHumidity();

        if (isnan(t) || isnan(h))
        {
            Serial.println("DHT Read Failed!");
            avgTemp = 0;
            avgHum = 0;
            return false;
        }

        tempReadings[i] = t;
        humReadings[i] = h;
        delay(200); // Задержка между измерениями
    }

    std::sort(tempReadings, tempReadings + 10);
    std::sort(humReadings, humReadings + 10);

    float tempSum = 0, humSum = 0;
    for (int i = 2; i < 8; i++)
    {
        tempSum += tempReadings[i];
        humSum += humReadings[i];
    }

    avgTemp = tempSum / 6;
    avgHum = humSum / 6;

    return true;
}


void loop()
{
    dht.begin();
    delay(500); // Для стабилизации работы датчика после запуска
    read_dht_data(myData.temperature, myData.humidity);
}
```

#### 🔎 Как это работает?

Функция `read_dht_data` считывает данные температуры и влажности с датчика DHT-22, выполняет 10 измерений, сортирует результаты и вычисляет средние значения, исключая крайние (2 минимальных и 2 максимальных).

Если чтение данных с датчика не удалось, возвращается `false`, а средние значения обнуляются.
