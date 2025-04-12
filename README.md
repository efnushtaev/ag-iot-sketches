# atsap-iot-sketches

Скетчи для IoT девайсов

## Набор IoT девайсов
1. [**esp32-controller** ESP32 с подключением к MQTT брокеру. ESP-NOW gateway](./esp32-controller/)
2. [**esp32-node-dht22** ESP32 + DHT-22. ESP-NOW node](./esp32-node-dht22/)

---

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

##### 🔎 Как это работает?

Функция `getWiFiChannel` определяет канал Wi-Fi сети по её SSID. Она выполняет сканирование доступных сетей с помощью `WiFi.scanNetworks()` и сравнивает SSID каждой найденной сети с переданным значением.

Если совпадение найдено, возвращается канал этой сети через `WiFi.channel(i)`. Если сеть не найдена или сканирование не удалось, функция возвращает `0`.

