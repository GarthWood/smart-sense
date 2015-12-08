#pragma once

#include "messages.pb.h"

#define HOST_NAME                 "10.4.108.22"
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
 Callback encoding string callback
*/
bool write_string(pb_ostream_t *stream, const pb_field_t *field, void * const **arg)
{
    char *str = (char*)stream->state;

    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)str, strlen(str));
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
        bool result = false;

        if (_client.connect(onConnectionState, forceApMode)) {
            // send the registration message
            //box.sendData();
            result = true;
        }

        return result;
    }

    /**
     Sends data to the server
    */
    void sendData(const char* path, int value) {

        uint8_t buffer[BUFFER_SIZE];
        ServiceMessage message = ServiceMessage_init_default;

        memset(buffer, 0, BUFFER_SIZE * sizeof(byte));
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, BUFFER_SIZE);

        SetNumber request;

        memset(&request, 0, sizeof(request));
        strcpy(request.path, path);
        request.value = value;

        message.which_payload = ServiceMessage_setNumber_tag;
        message.payload.setNumber = request;

        // encode and send the message
        if (pb_encode(&stream, ServiceMessage_fields, &message)) {
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
