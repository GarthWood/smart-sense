#ifndef SS_PROTOCOL_H_
#define SS_PROTOCOL_H_

#define PACKET_START                    0x1C
#define SHORT_STRING_MAX_LENGTH         256
#define SESSION_ID_LENGTH               16

enum PacketType {
    SetInteger = 10
};

/**
  The header definition which all packets will use as their starting characters
 */
typedef struct _Header {

    _Header() {
        start = PACKET_START;
        length = 0;
    }

    void initialise(PacketType packetType, uint16_t packetVersion) {
        type = (uint16_t)packetType;
        version = packetVersion;
    }

    int getSize() {
        return sizeof(start) + sizeof(type) + sizeof(version) + sizeof(length);
    }

    uint16_t start;
    uint16_t type;
    uint16_t version;
    uint32_t length;

} Header;


/**
  A packet to set an integer value
 */
#define SET_INTEGER_VERSION 0x01
typedef struct _SetInteger {

    _SetInteger() {
        memset(sessionId, 0, SESSION_ID_LENGTH);
        memset(path, 0, SHORT_STRING_MAX_LENGTH);
    }

    int getSize() {
        return header.getSize() + SESSION_ID_LENGTH + SHORT_STRING_MAX_LENGTH + sizeof(value);
    }

    uint8_t* getBuffer() {
        return reinterpret_cast<uint8_t*>(this);
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
