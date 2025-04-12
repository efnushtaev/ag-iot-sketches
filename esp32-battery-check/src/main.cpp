// ----------------------------------------------------------------------------
// --- EESP-NOW NODE ESP32 + DHT-22 ---
// ----------------------------------------------------------------------------

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ----------------------------------------------------------------------------
// WiFi
// ----------------------------------------------------------------------------

constexpr char WIFI_SSID[] = "TP-Link_B354";

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

void initWiFi()
{
    WiFi.mode(WIFI_MODE_STA);

    int32_t channel = getWiFiChannel(WIFI_SSID);

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
}

// ----------------------------------------------------------------------------
// ESP-NOW
// ----------------------------------------------------------------------------

typedef struct struct_message
{
    float temperature;
    float humidity;
    unsigned long timestamp;
} struct_message;

struct_message myData;

constexpr uint8_t ESP_NOW_RECEIVER[] = {0x3c, 0xe9, 0x0e, 0x8c, 0xf5, 0xd8};

esp_now_peer_info_t peerInfo;

void initEspNow()
{

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP NOW failed to initialize");
        while (1)
            ;
    }

    memcpy(peerInfo.peer_addr, ESP_NOW_RECEIVER, 6);
    peerInfo.ifidx = (wifi_interface_t)ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("ESP NOW pairing failure");
        while (1)
            ;
    }
}

// ----------------------------------------------------------------------------
// DHT-22
// ----------------------------------------------------------------------------

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

    // Сортировка массивов
    std::sort(tempReadings, tempReadings + 10);
    std::sort(humReadings, humReadings + 10);

    float tempSum = 0, humSum = 0;
    for (int i = 2; i < 8; i++)
    { // Используем только центральные 6 значений
        tempSum += tempReadings[i];
        humSum += humReadings[i];
    }

    avgTemp = tempSum / 6;
    avgHum = humSum / 6;

    return true;
}

void initDHT()
{
    dht.begin();

    delay(500); // Задержка для стабилизации DHT-22

    if (read_dht_data(myData.temperature, myData.humidity))
    {
        Serial.print("Средняя температура: ");
        Serial.println(myData.temperature);
        Serial.print("Средняя влажность: ");
        Serial.println(myData.humidity);

        // Отправка данных
        esp_err_t result = esp_now_send(ESP_NOW_RECEIVER, (uint8_t *)&myData, sizeof(myData));
        if (result == ESP_OK)
        {
            Serial.println("Данные успешно отправлены");
        }
        else
        {
            Serial.println("Ошибка отправки данных");
        }
    }
    else
    {
        Serial.println("Ошибка чтения данных с DHT-22");
    }
}
// ----------------------------------------------------------------------------
// INIT + LOOP
// ----------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    initWiFi();
    initEspNow();
}

uint32_t last;
const unsigned long interval = 5000;

void loop()
{
    if (millis() - last > interval)
    {
        myData.timestamp = millis();
        initDHT();
        last = millis();
    }
}