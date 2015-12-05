#pragma once

#include "UDPClient.hpp"

#define STATUS_LED_PIN            BUILTIN_LED
#define AP_PIN                    5
#define SERVER_PORT               11000

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

    struct ConnectionDetails {
        int localPort;

        void save(int localPort) {
            this->localPort = localPort;
        }
    };

public:
    Box(const char* apName, const char* host, int bufferSize = 48) {

        pinMode(STATUS_LED_PIN, OUTPUT);
        pinMode(AP_PIN, INPUT);

        _client = new UDPClient(apName, host, SERVER_PORT, bufferSize);
    }

    ~Box() {
        delete _client;
    }

    /**
     Connects to the Wifi, initiates the UDP connection and sets up the box
    */
    bool connect(int localPort) {

        bool forceApMode = (digitalRead(AP_PIN) == HIGH);

        _connectionDetails.save(localPort);

        return _client->connect(localPort, onConnectionState, forceApMode);
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
            connect(_connectionDetails.localPort);
        }

        return _client->isConnected();
    }

private:

    UDPClient* _client;

    ConnectionDetails _connectionDetails;

};
