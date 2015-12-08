
// ESP
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiUdp.h>

// nanopb
#include "pb_common.hpp"
#include "pb_encode.hpp"
#include "pb_decode.hpp"

// box specific
#include "WiFiManager.hpp"
#include "UDPClient.hpp"
#include "Box.hpp"

// messages
#include "simple.pb.hpp"


#define AP_NAME           "TemperatureBox"
#define POT_PIN           A0
#define SEND_INTERVAL     1000

void onMessage(stMessage& message);
int readPotValue();

const int NUM_READINGS = 50;
int readings[NUM_READINGS];
int readingIndex = 0;
int currentReading = 0;
int lastReading = 0;

long potTimer = 0;

Box box(AP_NAME, onMessage);


void setup() {

    Serial.begin(9600);

    potTimer = millis();

    if (box.connect()) {
        Serial.println("Connected");
    } else {
        Serial.println("Could not establish connection");
    }
}

void loop() {

    if (box.run()) {

        /*char* buffer = NULL;

        if (box.getData(buffer)) {
            Serial.println(buffer);
            digitalWrite(LED, String(buffer) != "0" ? HIGH : LOW);
            box.sendData("test");
        }*/
    }
}

void sendPotValue() {

    long now = millis();

    if ((now - potTimer) >= SEND_INTERVAL) {

        if (lastReading != currentReading) {
            box.sendData((long)currentReading);
            lastReading = currentReading;
        }

        potTimer = now;
    }
}

int readPotValue() {

    if (readingIndex == NUM_READINGS) {

        int total = 0;

        for (int i = 0; i < NUM_READINGS; ++i) {
            total += readings[i];
        }

        currentReading = (int)(total / NUM_READINGS);
        readingIndex = 0;
    } else {
        readings[readingIndex++] = analogRead(POT_PIN);
        delay(1);
    }

    return currentReading;
}

void onMessage(stMessage& message) {

    switch (message.type) {
        default:
            break;
    }
}
