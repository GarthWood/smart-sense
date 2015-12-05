
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include "WiFiManager.hpp"
#include "Box.hpp"

#define AP_NAME           "LEDBox"

#define LED               15

Box box(AP_NAME);

void setup() {

    Serial.begin(115200);

    pinMode(LED, OUTPUT);

    if (box.connect()) {
        Serial.println("Connected");
    } else {
        Serial.println("Could not establish connection");
    }

    box.sendData("test");
}

void loop() {

    if (box.run()) {

        char* buffer = NULL;

        if (box.getData(buffer)) {
            Serial.println(buffer);
            digitalWrite(LED, String(buffer) != "0" ? HIGH : LOW);
            box.sendData("test");
        }
    }
}
