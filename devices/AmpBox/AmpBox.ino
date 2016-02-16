
// ESP
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>

// box specific
#include <OneWire.h>
#include "WiFiManager.hpp"
#include "UDPClient.hpp"
#include "Box.hpp"
#include "Temperature.hpp"
#include "Moisture.hpp"

// protocol
#include "ss_protocol.h"


#define AP_NAME           "TemperatureBox2"
#define PATH              "/devices/TemperatureBox2/output/value1"
#define SEND_INTERVAL     1000

void onMessage(stMessage& message);
void sendValues();

long timer = 0;

Box box(AP_NAME, onMessage);
Temperature temperature(true);
Moisture moisture(false);


void setup() {

    Serial.begin(9600);

    timer = millis();

    if (box.connect()) {
        Serial.println("Connected");
        temperature.init();
        moisture.init();
    } else {
        Serial.println("Could not establish connection");
    }
}

void loop() {

    if (box.run()) {
        temperature.run();
        moisture.run();
        sendValues();
    }
}

void sendValues() {

    long now = millis();

    if ((now - timer) >= SEND_INTERVAL) {

        // temperature
        if (temperature.hasChanged()) {
            box.sendData(PATH, (int)temperature.read());
        }

        // moisture
        if (moisture.hasChanged()) {
            box.sendData(PATH, (int)moisture.read());
        }

        timer = now;
    }
}

void onMessage(stMessage& message) {

    switch (message.type) {
        default:
            break;
    }
}
