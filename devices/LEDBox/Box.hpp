#pragma once

#include "simple.pb.h"

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
    Box(const char* apName) {

        pinMode(STATUS_LED_PIN, OUTPUT);
        pinMode(AP_PIN, INPUT);

        _client.init(apName, HOST_NAME, REMOTE_PORT);
    }

    ~Box() {
    }

    /**
     Connects to the Wifi, initiates the UDP connection and sets up the box
    */
    bool connect() {

        bool forceApMode = (digitalRead(AP_PIN) == HIGH);

        return _client.connect(onConnectionState, forceApMode);
    }

    /**
     Returns any data in the buffer
    */
    bool getData(char*& data) {
        //return _client.getData(data);
        return false;
    }

    /**
     Sends data to the server
    */
    void sendData(const char* packet) {

        uint8_t buffer[BUFFER_SIZE];
        SimpleMessage message = SimpleMessage_init_zero;

        memset(buffer, 0, BUFFER_SIZE * sizeof(byte));

        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

        //message.data = packet;

        /* Now we are ready to encode the message! */
        if (pb_encode(&stream, SimpleMessage_fields, &message)) {
            _client.sendData((const char*)buffer);
        }
    }

    /**
     Primary run loop for the box
    */
    bool run() {

        if (!_client.isConnected()) {
            connect();
        } else {
            handleIncomingMessages();
        }

        return _client.isConnected();
    }

private:

    UDPClient _client;

    /**
     Processes messages send from the server
    */
    void handleIncomingMessages() {

        uint8_t* buffer = NULL;
        size_t length = (size_t)_client.getData((char*&)buffer);

        if (length > 0) {
            SimpleMessage message = SimpleMessage_init_zero;

            pb_istream_t stream = pb_istream_from_buffer(buffer, length);

            if (pb_decode(&stream, SimpleMessage_fields, &message)) {
                //message.x;
            }
        }
    }
};
