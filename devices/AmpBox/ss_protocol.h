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

    void fillBuffer(ss_buffer*& buffer) {

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

    _SetInteger() {
        memset(sessionId, 0, SESSION_ID_LENGTH);
        memset(path, 0, SHORT_STRING_MAX_LENGTH);
    }

    uint8_t* fillBuffer(ss_buffer* buffer) {

        uint8_t* startPointer = buffer;

        // clear the buffer
        memset(buffer, 0, SET_INTEGER_SIZE);

        // header
        header.fillBuffer(buffer);

        // session ID
        memcpy(buffer, sessionId, SESSION_ID_LENGTH);
        buffer += SESSION_ID_LENGTH;

        // path
        memcpy(buffer, &path, SHORT_STRING_MAX_LENGTH);
        buffer += SHORT_STRING_MAX_LENGTH;

        // value
        memcpy(buffer, &value, sizeof(value));

        return startPointer;
    }

    Header header;
    char sessionId[SESSION_ID_LENGTH];
    char path[SHORT_STRING_MAX_LENGTH];
    int32_t value;

} SetIntegerPacket;

/**
  Helper function to create a SetInteger packet
 */
bool createSetIntegerPacket(SetIntegerPacket& packet, const char* sessionId, const char* path, int32_t value) {

    int pathLength = strlen(path);
    bool valid = (pathLength > 0) && (pathLength <= SHORT_STRING_MAX_LENGTH);

    if (valid) {

        // header
        packet.header.initialise(SetInteger, SET_INTEGER_VERSION);

        // session ID
        memcpy(packet.sessionId, sessionId, strlen(sessionId));

        // path
        memcpy(packet.path, path, pathLength);

        // value
        packet.value = value;
    }

    return valid;
}

#endif // SS_PROTOCOL_H_
