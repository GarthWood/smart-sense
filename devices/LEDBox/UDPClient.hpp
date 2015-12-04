#pragma once

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
  }

  bool connect(int localPort, const char* ssid, const char* password) {

    bool result = false;

    if (connect(ssid, password)) {
      _udp.begin(localPort);
      WiFi.hostByName(_host.c_str(), _ip);
      result = true;
    }

    return result;
  }

  const char* getData() {

    memset(_inputBuffer, 0, _bufferSize * sizeof(byte));
    
    if (_udp.parsePacket()) {
      _udp.read(_inputBuffer, _bufferSize);
    }

    return (const char*)_inputBuffer;
  }

  void sendData(const char* packet) {

    memset(_outputBuffer, 0, _bufferSize * sizeof(byte));
    memcpy(_outputBuffer, packet, strlen(packet) + 1);
   
    _udp.beginPacket(_ip, _serverPort);
    _udp.write(_outputBuffer, _bufferSize);
    _udp.endPacket();
  }

private:

  bool connect(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }

    return true;
  }

  String _host;

  WiFiUDP _udp;
  
  IPAddress _ip;
  
  byte *_inputBuffer,
       *_outputBuffer;

  int _bufferSize;

  int _serverPort;
};

