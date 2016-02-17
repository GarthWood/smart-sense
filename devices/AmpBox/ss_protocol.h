#ifndef SS_PROTOCOL_H_
#define SS_PROTOCOL_H_

#define PACKET_START                    28
#define SHORT_STRING_MAX_LENGTH         256
#define SESSION_ID_LENGTH               16

enum PacketType {
    SetInteger = 10
};

/**
  The header definition which all packets will use as their starting characters
 */
#define HEADER_SIZE (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t))
typedef struct _Header {

    _Header() {
        start = PACKET_START;
        length = 0;
    }

    void initialise(PacketType packetType, uint16_t packetVersion) {
        type = (uint16_t)packetType;
        version = packetVersion;
    }

    void fillBuffer(uint8_t*& buffer) {

        // start
        memcpy(buffer, &start, sizeof(start));
        buffer += sizeof(start);

        // type
        memcpy(buffer, &type, sizeof(type));
        buffer += sizeof(type);

        // version
        memcpy(buffer, &version, sizeof(version));
        buffer += sizeof(version);

        // length
        memcpy(buffer, &length, sizeof(length));
        buffer += sizeof(length);
    }

    uint16_t start;
    uint16_t type;
    uint16_t version;
    uint32_t length;

} Header;


/**
  A packet to set an integer value
 */
#define SET_INTEGER_VERSION 1
#define SET_INTEGER_SIZE (HEADER_SIZE + SESSION_ID_LENGTH + SHORT_STRING_MAX_LENGTH + sizeof(int32_t))
typedef struct _SetInteger {

    _SetInteger(const char* sessionId, const char* path, int32_t value) {

        uint8_t* pBuffer = buffer;

        // clear the buffer
        memset(buffer, 0, SET_INTEGER_SIZE);

        // header
        header.initialise(SetInteger, SET_INTEGER_VERSION);
        header.fillBuffer(pBuffer);

        // session ID
        memcpy(pBuffer, sessionId, strlen(sessionId));
        pBuffer += SESSION_ID_LENGTH;

        // path
        memcpy(pBuffer, path, strlen(path));
        pBuffer += SHORT_STRING_MAX_LENGTH;

        // value
        memcpy(pBuffer, &value, sizeof(int32_t));
    }

    uint8_t* getBuffer() {
        return buffer;
    }

    int getSize() {
        return SET_INTEGER_SIZE;
    }

    Header header;
    uint8_t buffer[SET_INTEGER_SIZE];

} SetIntegerPacket;

#endif // SS_PROTOCOL_H_
