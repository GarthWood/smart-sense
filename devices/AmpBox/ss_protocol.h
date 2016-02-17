#ifndef SS_PROTOCOL_H_
#define SS_PROTOCOL_H_

#define PACKET_START                    28
#define SHORT_STRING_MAX_LENGTH         256
#define SESSION_ID_LENGTH               16

typedef uint8_t ss_buffer;

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

    _SetInteger(const char* packetSessionId, const char* packetPath, int32_t packetValue) {

        memset(sessionId, 0, SESSION_ID_LENGTH);
        memset(path, 0, SHORT_STRING_MAX_LENGTH);
        memset(buffer, 0, SET_INTEGER_SIZE);

        // header
        header.initialise(SetInteger, SET_INTEGER_VERSION);

        // session ID
        memcpy(sessionId, packetSessionId, strlen(packetSessionId));

        // path
        memcpy(path, packetPath, strlen(packetPath));

        // value
        value = packetValue;
    }

    uint8_t* getBuffer() {

        uint8_t* startPointer = buffer;
        uint8_t* modPointer = buffer;

        // header
        header.fillBuffer(modPointer);

        // session ID
        memcpy(modPointer, sessionId, SESSION_ID_LENGTH);
        modPointer += SESSION_ID_LENGTH;

        // path
        memcpy(modPointer, &path, SHORT_STRING_MAX_LENGTH);
        modPointer += SHORT_STRING_MAX_LENGTH;

        // value
        memcpy(modPointer, &value, sizeof(value));

        return startPointer;
    }

    uint8_t buffer[SET_INTEGER_SIZE];

    Header header;
    char sessionId[SESSION_ID_LENGTH];
    char path[SHORT_STRING_MAX_LENGTH];
    int32_t value;

} SetIntegerPacket;

#endif // SS_PROTOCOL_H_
