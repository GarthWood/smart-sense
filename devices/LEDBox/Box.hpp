#pragma once

#include "UDPClient.hpp"

#define STATUS_INTERVAL           1000
#define STATUS_LED_PIN            5

/**
 A general container for any device
*/
class Box {

    enum State {
        None = 0,
        Connecting,
        Connected
    };

    struct ConnectionDetails {
        int localPort;

        void save(int localPort) {
            this->localPort = localPort;
        }
    };

public:
    Box(const char* apName, const char* host, int serverPort, int bufferSize = 48)
    : _statusPin(STATUS_LED_PIN)
    , _state(None)
    , _statusTimer(0L)
    , _statusState(false) {

        _client = new UDPClient(apName, host, serverPort, bufferSize);
    }

    ~Box() {
        delete _client;
    }

    /**
     Connects to the Wifi, initiates the UDP connection and sets up the box
    */
    bool connect(int localPort) {

        _state = Connecting;

        _connectionDetails.save(localPort);

        pinMode(_statusPin, OUTPUT);

        if (_client->connect(localPort)) {
            _state = Connected;
        }

        _statusTimer = millis();

        return (_state == Connected);
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

        if (isConnected()) {
            // status indicator
            processStatusIndicator();
        } else {
            connect(_connectionDetails.localPort);
        }

        return isConnected();
    }

private:

    UDPClient* _client;

    ConnectionDetails _connectionDetails;

    int _statusPin;

    State _state;

    long _statusTimer;
    bool _statusState;

    /**
     Returns the connection state of the box
    */
    bool isConnected() {
        return (_state == Connected) && _client->isConnected();
    }

    /**
     Handles the updates to the status indicator
    */
    void processStatusIndicator() {

        long now = millis();

        if ((now - _statusTimer) >= STATUS_INTERVAL) {

            // handle the state
            switch (_state) {
                case Connecting:
                    _statusState = !_statusState;
                    digitalWrite(_statusPin, _statusState ? HIGH : LOW);
                    break;
                case Connected:
                    digitalWrite(_statusPin, HIGH);
                    break;
                default:
                    digitalWrite(_statusPin, LOW);
                    break;
            }

            // reset the timer
            _statusTimer = now;
        }
    }
};
