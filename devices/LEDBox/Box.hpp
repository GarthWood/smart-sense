#pragma once

#include "UDPClient.hpp"

#define HOST_NAME                 "10.4.108.22"
#define REMOTE_PORT               11000

#define STATUS_LED_PIN            BUILTIN_LED
#define AP_PIN                    5

/**
 Global callback to update the status LED based on the connection state
*/
void onConnectionState(bool connected) {

    static bool state = false;

    if (connected) {
        digitalWrite(STATUS_LED_PIN, HIGH);
    } else {
        state = !state;
        digitalWrite(STATUS_LED_PIN, state ? LOW : HIGH);
    }
}

/**
 A general container for any device
*/
class Box {

public:
    Box(const char* apName, int bufferSize = 48) {

        pinMode(STATUS_LED_PIN, OUTPUT);
        pinMode(AP_PIN, INPUT);

        _client = new UDPClient(apName, HOST_NAME, REMOTE_PORT, bufferSize);
    }

    ~Box() {
        delete _client;
    }

    /**
     Connects to the Wifi, initiates the UDP connection and sets up the box
    */
    bool connect() {

        bool forceApMode = (digitalRead(AP_PIN) == HIGH);

        return _client->connect(onConnectionState, forceApMode);
    }

    /**
     Returns any data in the buffer
    */
    bool getData(char*& data) {
        return _client->getData(data);
    }

    /**
     Sends data to the server
    */
    void sendData(const char* packet) {
        return _client->sendData(packet);
    }

    /**
     Primary run loop for the box
    */
    bool run() {

        if (!_client->isConnected()) {
            connect();
        }

        return _client->isConnected();
    }

private:

    UDPClient* _client;

};
