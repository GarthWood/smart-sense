
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "UDPClient.h"

#define SSID              "DVT Durban"
#define PASSWORD          "wheee wheee wheee!"
#define HOST_NAME         "10.4.108.22"
#define LOCAL_PORT        2390

#define LED               15

UDPClient client(HOST_NAME, 33333, 48);

void setup() {

  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  if (client.connect(LOCAL_PORT, SSID, PASSWORD)) {
    Serial.println("Connected");
  } else {
    Serial.println("Could not establish connection");
  }

  client.sendData("test");
}

void loop() {

  String data = client.getData();

  if (data != "") {
    Serial.println(data);
    digitalWrite(LED, String(data) != "0" ? HIGH : LOW);
    client.sendData("test");
  }
}
