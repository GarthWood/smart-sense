/**
 * Represents a message type
 */
var MessageType = {
    PING: 'ping',
    PONG: 'pong',
    SUBSCRIBE: 'subscribe',
    UNSUBSCRIBE: 'unsubscribe',
    GET: 'get',
    ERROR: 'error',
    FLOAT_VALUE: 'floatValue',
    SET_FLOAT: 'setFloat'
};

module.exports = MessageType;
