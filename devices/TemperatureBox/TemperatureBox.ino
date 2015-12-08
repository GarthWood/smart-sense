
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
#include <OneWire.h>
#include "WiFiManager.hpp"
#include "UDPClient.hpp"
#include "Box.hpp"

// messages
#include "messages.pb.hpp"


#define AP_NAME           "TemperatureBox"
#define TEMP_PIN          13
#define SEND_INTERVAL     1000

void onMessage(stMessage& message);
int readPotValue();
void setupDS18B20();
void sendTemperature();
void readTemperature();

const int NUM_READINGS = 5;
int readings[NUM_READINGS];
int readingIndex = 0;
int currentReading = 0;
int lastReading = 0;

long timer = 0;

Box box(AP_NAME, onMessage);

OneWire tempSensor(TEMP_PIN);
byte sensorAddress[8];

void setup() {

    Serial.begin(9600);

    timer = millis();

    if (box.connect()) {
        Serial.println("Connected");
        setupDS18B20();
    } else {
        Serial.println("Could not establish connection");
    }
}

void loop() {

    if (box.run()) {
        readTemperature();
        sendTemperature();
    }
}

void setupDS18B20() {

    while (!tempSensor.search(sensorAddress)) {
        tempSensor.reset_search();
        memset(sensorAddress, 0, 8);
    }
}

void sendTemperature() {

    long now = millis();

    if ((now - timer) >= SEND_INTERVAL) {

        if (lastReading != currentReading) {
            box.sendData("box0001/outputEvents/value", currentReading);
            lastReading = currentReading;
        }

        timer = now;
    }
}

void readTemperature() {

    if (readingIndex == NUM_READINGS) {

        int total = 0;

        for (int i = 0; i < NUM_READINGS; ++i) {
            total += readings[i];
        }

        currentReading = (int)(total / NUM_READINGS);
        readingIndex = 0;
    } else {

        byte data[12];

        tempSensor.reset();
        tempSensor.select(sensorAddress);
        tempSensor.write(0x44, 1);
        delay(750);
        tempSensor.reset();
        tempSensor.select(sensorAddress);
        tempSensor.write(0xBE);

        for (int i = 0; i < 9; ++i) {           // we need 9 bytes
            data[i] = tempSensor.read();
        }

        int16_t raw = (data[1] << 8) | data[0];

        byte cfg = (data[4] & 0x60);

        if (cfg == 0x00) {
            raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        } else if (cfg == 0x20) {
            raw = raw & ~3; // 10 bit res, 187.5 ms
        } else if (cfg == 0x40) {
            raw = raw & ~1; // 11 bit res, 375 ms
        }

        readings[readingIndex++] = (int)(((float)raw / 16.0) * 1000);
    }
}

void onMessage(stMessage& message) {

    switch (message.type) {
        default:
            break;
    }
}
