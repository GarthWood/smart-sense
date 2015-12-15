#pragma once

#include "messages.pb.h"

#define HOST_NAME                 "192.168.0.11"//"10.4.108.22"
#define REMOTE_PORT               11000

#define STATUS_LED_PIN            4
#define AP_PIN                    5

struct stMessage {
    int type;
    int nm;
    float fm;
    char* cm;
};

typedef void (*messageCallback)(stMessage&);

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
    Box(const char* apName, messageCallback callback)
    : _messageCallback(callback) {

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
     Sends data to the server
    */
    void sendData(const char* path, int value) {

        uint8_t buffer[SetNumber_size];
        ServiceMessage message;

        memset(buffer, 0, SetNumber_size * sizeof(byte));
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, SetNumber_size);

        memset(message.payload.setNumber.path, 0, sizeof(message.payload.setNumber.path));
        strcpy(message.payload.setNumber.path, path);
        message.payload.setNumber.value = value;

        message.has_messageId = false;
        message.which_payload = ServiceMessage_setNumber_tag;

        // encode and send the message
        if (pb_encode(&stream, ServiceMessage_fields, &message)) {
            _client.sendData(buffer, SetNumber_size);
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

    messageCallback _messageCallback;

    /**
     Processes messages send from the server
    */
    void handleIncomingMessages() {

        /*uint8_t* buffer = NULL;
        size_t length = (size_t)_client.getData((char*&)buffer);

        if (length > 0) {
            SimpleMessage message = SimpleMessage_init_zero;

            pb_istream_t stream = pb_istream_from_buffer(buffer, length);

            if (pb_decode(&stream, SimpleMessage_fields, &message)) {

                stMessage message;

                message.type = x;
                message.nm = x;

                _messageCallback(message);
            }
        }*/
    }
};
