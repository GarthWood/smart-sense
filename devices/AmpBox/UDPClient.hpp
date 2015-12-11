#pragma once

#include "WiFiManager.hpp"

#define LOCAL_PORT        2390
#define BUFFER_SIZE       128

/**
 A general purpose UDP client
*/
class UDPClient {

public:
    UDPClient() {
    }

    ~UDPClient() {
    }

    /**
     Initialises the client
    */
    void init(const char* apName, const char* host, int serverPort) {

        _ap = apName;
        _host = host;
        _serverPort = serverPort;

        _wifi.setDebugOutput(false);
    }

    /**
     Connects to the Wifi and initiates the UDP connection
    */
    bool connect(connectionStateCallback callback, bool forceApMode) {

        bool result = false;

        if (_udp) {
            _udp.stopAll();
        }

        if (autoConnect(callback, forceApMode)) {
            _udp.begin(LOCAL_PORT);
            WiFi.hostByName(_host.c_str(), _ip);
            result = true;
        }

        return result;
    }

    /**
     Returns any data in the buffer
    */
    int getData(char*& data) {

        int packetLength = _udp.parsePacket();

        if (packetLength > 0) {
            memset(_inputBuffer, 0, BUFFER_SIZE * sizeof(byte));
            _udp.read(_inputBuffer, BUFFER_SIZE);
            data = (char*)_inputBuffer;
        }

        return packetLength;
    }

    /**
     Sends data to the server
    */
    void sendData(uint8_t* packet, int size) {

        _udp.beginPacket(_ip, _serverPort);
        _udp.write(packet, size);
        _udp.endPacket();
    }

    /**
     Returns if the Wifi is connected
    */
    bool isConnected() {
        return (WiFi.status() == WL_CONNECTED);
    }

private:

    String _host,
           _ap;

    WiFiManager _wifi;

    WiFiUDP _udp;

    IPAddress _ip;

    byte _inputBuffer[BUFFER_SIZE];

    int _serverPort;

    /**
     Establishes the connection to the Wifi
    */
    bool autoConnect(connectionStateCallback callback, bool forceApMode) {
        return _wifi.autoConnect(_ap.c_str(), callback, forceApMode);
    }
};
