#pragma once

#define CONNECTION_TIMEOUT          60

/**
A general purpose UDP client
*/
class UDPClient {

public:
    UDPClient(const char* apName, const char* host, int serverPort, int bufferSize) {
        _inputBuffer = (byte*)malloc(bufferSize * sizeof(byte));
        _outputBuffer = (byte*)malloc(bufferSize * sizeof(byte));
        _ap = apName;
        _host = host;
        _serverPort = serverPort;
        _bufferSize = bufferSize;
    }

    ~UDPClient() {
        free(_inputBuffer);
        free(_outputBuffer);
    }

    /**
    Connects to the Wifi and initiates the UDP connection
    */
    bool connect(int localPort) {

    bool result = false;

        _udp.stop();

        if (autoConnect()) {
            _udp.begin(localPort);
            WiFi.hostByName(_host.c_str(), _ip);
        }

        return result;
    }

    /**
    Returns any data in the buffer
    */
    bool getData(char*& data) {

        bool hasData = _udp.parsePacket() != 0;

        if (hasData) {
            memset(_inputBuffer, 0, _bufferSize * sizeof(byte));
            _udp.read(_inputBuffer, _bufferSize);
            data = (char*)_inputBuffer;
        }

        return hasData;
    }

    /**
    Sends data to the server
    */
    void sendData(const char* packet) {

        memset(_outputBuffer, 0, _bufferSize * sizeof(byte));
        memcpy(_outputBuffer, packet, strlen(packet) + 1);

        _udp.beginPacket(_ip, _serverPort);
        _udp.write(_outputBuffer, _bufferSize);
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

    byte *_inputBuffer,
       *_outputBuffer;

    int _bufferSize;

    int _serverPort;

    /**
    Establishes the connection to the Wifi
    */
    bool autoConnect() {

        _wifi.setTimeout(CONNECTION_TIMEOUT);
        _wifi.autoConnect(_ap.c_str());

        return isConnected();
    }
};
