
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Box.hpp"

#define SSID              "***"
#define PASSWORD          "***"
#define HOST_NAME         "10.4.108.22"
#define LOCAL_PORT        2390
#define STATUS_PIN        15

#define LED               15

Box box(STATUS_PIN, HOST_NAME, 33333, 48);

void setup() {

  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  if (box.connect(LOCAL_PORT, SSID, PASSWORD)) {
    Serial.println("Connected");
  } else {
    Serial.println("Could not establish connection");
  }

  box.sendData("test");
}

void loop() {

    // run the box first
    if (box.run()) {

        char* buffer = NULL;

        if (box.getData(buffer)) {
            Serial.println(buffer);
            digitalWrite(LED, String(buffer) != "0" ? HIGH : LOW);
            box.sendData("test");
        }
    }
}
