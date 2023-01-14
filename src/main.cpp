#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <CustomStepper.h>
#include "wifi/wifi_connecting.h"
#include "config/settings.h"
#include "secret/secret.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

CustomStepper stepper(STEPPER_A, STEPPER_B, STEPPER_C, STEPPER_D);

char ssid[] = WIFI_SSID;
char password[] = WIFI_PASSWORD;

const char mqtt_server[] = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;

String generate_client_id()
{
    String client_id = "ESP32SmartPillIoT";
    client_id += (random(0xffff), HEX);
    return client_id;
}

void mqtt_setup()
{
    mqtt_client.setServer(mqtt_server, mqtt_port);
    // mqtt_client.setCallback(/*here will be callback method*/);
}

void mqtt_reconnect()
{
    Serial.println("Connecting to MQTT broker...");
    while (!mqtt_client.connected())
    {
        Serial.println("Reconnecting to MQTT broker...");
        if (mqtt_client.connect(generate_client_id().c_str()), USERNAME_CREDENTIAL, PASSWORD_CREDENTIAL)
        {
            Serial.println("Connected!");
            mqtt_client.subscribe(SUB_TOPIC);
        }
    }
}

int get_tilt_state()
{
    return digitalRead(TILT_SENSOR);
}

void check_tilt_state_and_send_message_to_mqtt() {
    if (get_tilt_state() == HIGH) {
        char tilt_array[16];
        dtostrf(get_tilt_state(), 1, 2, tilt_array);
        mqtt_client.publish(PUB_TOPIC, tilt_array);
    }
}

void stepper_control() {
    if (stepper.isDone())
    {
        stepper.setDirection(CW);
        stepper.rotateDegrees(90);
        stepper.setDirection(STOP);
    }
    stepper.run();
}

void setup()
{
    Serial.begin(115200);
    wifi_connection(ssid, password);
    mqtt_setup();
    pinMode(TILT_SENSOR, INPUT);
    stepper.setRPM(12);
    stepper.setSPR(4075.7728395);
}

void loop()
{
    if (!mqtt_client.connected())
        mqtt_reconnect();
    mqtt_client.loop();
    check_tilt_state_and_send_message_to_mqtt();
    stepper_control();
}