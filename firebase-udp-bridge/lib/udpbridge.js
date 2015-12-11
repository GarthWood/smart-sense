'use strict';

var dgram = require('dgram'),
    colors = require('colors'),
    MessageUtility = require('./messageutility.js'),
    MessageType = require('./lookups/messagetype.js'),
    ErrorCode = require('./lookups/errorcode.js'),
    ErrorMessage = require('./types/errormessage.js'),
    ResponseMessage = require('./types/responsemessage.js'),
    ProtocolProvider = require('./protocolprovider.js'),
    q = require('Q');

/**
 * Creates a new UDP bridge service
 * @param {Number} port the listen port
 * @param {./SubscriptionService.js} subscriptionService the subscription service
 * @param {./QueryService.js} queryService the data query service
 * @param {./PresenceService.js} presenceService the client presence service
 * @constructor
 */
function UDPBridge(port,
                   subscriptionService,
                   queryService,
                   presenceService) {

    var server = dgram.createSocket('udp4'),
        address;

    var ServiceMessage = ProtocolProvider.createServiceMessage();

    function init() {
        server.on('listening', onServerListening);
        server.on('message', onIncomingMessage);
        server.bind(port);
    }

    function onServerListening() {
        address = server.address();
        console.log('Server running at => ' + colors.green(address.address + ':' + address.port));
    }

    function onIncomingMessage(incomingBuffer, client) {

        var responsePromise;
        var message = ServiceMessage.decode(incomingBuffer);
        var type = MessageUtility.getMessageType(message);

        switch (type) {
            case MessageType.PING:
                responsePromise = presenceService.ping(message.ping, client);
                break;
            case MessageType.SUBSCRIBE:
                responsePromise = subscriptionService.subscribe(message.subscribe, client);
                break;
            case MessageType.UNSUBSCRIBE:
                responsePromise = subscriptionService.unsubscribe(message.unsubscribe, client);
                break;
            case MessageType.GET:
                responsePromise = queryService.get(message.get, client);
                break;
            case MessageType.SET_NUMBER:
                responsePromise = queryService.setNumber(message.setNumber, client);
                break;
            case MessageType.LOG_MESSAGE:
                var log = message.logMessage;
                logDeviceMessage(log.deviceId, log.text);
                responsePromise = q();
                break;
            default:
                var response = new ResponseMessage(MessageType.ERROR,
                    new ErrorMessage(ErrorCode.UNKNOWN_MESSAGE_TYPE, type));
                responsePromise = q.resolve(response);
        }

        responsePromise.then(function success(response) {
            if (response) {
                sendMessage(response.type, response.payload, client.port, client.address);
            }
        }).done();
    }

    function sendMessage(type, message, remotePort, remoteAddress) {
        var serviceMessage = new ServiceMessage();
        serviceMessage.set(type, message);
        var outgoingBuffer = serviceMessage.encodeNB();
        server.send(outgoingBuffer, 0, outgoingBuffer.length, remotePort, remoteAddress);
    }

    function logDeviceMessage(deviceId, text) {
        console.log(colors.magenta(deviceId + ': ') + text);
    }

    init();

    // Public API
    return {
    };
}

module.exports = UDPBridge;
