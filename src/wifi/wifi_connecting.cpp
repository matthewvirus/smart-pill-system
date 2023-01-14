#include "wifi_connecting.h"

void wifi_connection(char ssid[], char password[])
{
    Serial.printf("Connection to %s\n", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.print("Connected!");
}