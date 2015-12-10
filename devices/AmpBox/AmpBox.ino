
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
#include "Temperature.hpp"
#include "Moister.hpp"

// messages
#include "messages.pb.hpp"


#define AP_NAME           "TemperatureBox"
#define SEND_INTERVAL     1000

void onMessage(stMessage& message);
void sendValues();

long timer = 0;

Box box(AP_NAME, onMessage);
Temperature temperature(true);
Moister moister(false);


void setup() {

    Serial.begin(9600);

    timer = millis();

    if (box.connect()) {
        Serial.println("Connected");
        temperature.init();
        moister.init();
    } else {
        Serial.println("Could not establish connection");
    }
}

void loop() {

    if (box.run()) {
        temperature.run();
        moister.run();
        sendValues();
    }
}

void sendValues() {

    long now = millis();

    if ((now - timer) >= SEND_INTERVAL) {

        // temperature
        if (temperature.hasChanged()) {
            box.sendData("box0001/outputEvents/value", (int)(temperature.read() * 1000));
        }

        // moister
        if (moister.hasChanged()) {
            //box.sendData("box0001/outputEvents/value", (int)(moister.read() * 1000));
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
