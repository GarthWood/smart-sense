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
    NUMBER_VALUE: 'numberValue',
    SET_NUMBER: 'setNumber',
    LOG_MESSAGE: 'logMessage'
};

module.exports = MessageType;
