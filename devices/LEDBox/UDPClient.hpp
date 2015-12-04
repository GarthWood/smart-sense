#pragma once

/**
 A general purpose UDP client
*/
class UDPClient {

public:
  UDPClient(const char* host, int serverPort, int bufferSize) {
    _inputBuffer = (byte*)malloc(bufferSize * sizeof(byte));
    _outputBuffer = (byte*)malloc(bufferSize * sizeof(byte));
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
  bool connect(int localPort, const char* ssid, const char* password) {

    bool result = false;

    if (connect(ssid, password)) {
      _udp.begin(localPort);
      WiFi.hostByName(_host.c_str(), _ip);
      result = true;
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

  String _host;

  WiFiUDP _udp;
  
  IPAddress _ip;
  
  byte *_inputBuffer,
       *_outputBuffer;

  int _bufferSize;

  int _serverPort;

  /**
   Establishes the connection to the Wifi
  */
  bool connect(const char* ssid, const char* password) {

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }

    return true;
  }
};
