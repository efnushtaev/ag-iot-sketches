// ----------------------------------------------------------------------------
// --- ESP32 NODE ESP-NOW TEMPLATE ---
// ----------------------------------------------------------------------------

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "config.h"

// ----------------------------------------------------------------------------
// WiFi
// ----------------------------------------------------------------------------

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

void sendEspNowData()
{
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

// ----------------------------------------------------------------------------
// INIT + LOOP
// ----------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    initWiFi();
    initEspNow();
}

void loop()
{
    myData.timestamp = millis();
    sendEspNowData();

    delay(1000);
}