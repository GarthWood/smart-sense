'use strict';

var dgram = require('dgram'),
    colors = require('colors'),
    Utility = require('./utility.js'),
    MessageProcessor = require('./MessageProcessor.js'),
    MessageTypes = require('./Lookups.js').MessageTypes;


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

    function init() {
        server.on('listening', onServerListening);
        server.on('message', onIncomingMessage);
    }

    function onServerListening() {
        address = server.address();
        console.log('Server running at\n  => ' + colors.green('http://' + address + ':' + port));
    }

    function onIncomingMessage(incomingBuffer, client) {
        var message = MessageProcessor.toMessage(incomingBuffer);

        var responsePromise;

        switch (message.type) {
            case MessageTypes.PING:
                responsePromise = presenceService.ping(message, client);
                break;
            case MessageTypes.SUBSCRIBE:
                responsePromise = subscriptionService.subscribe(message, client);
                break;
            case MessageTypes.UNSUBSCRIBE:
                responsePromise = subscriptionService.unsubscribe(message, client);
                break;
            case MessageTypes.GET:
                responsePromise = queryService.get(message, client);
                break;
            case MessageTypes.SET:
                responsePromise = queryService.set(message, client);
                break;
            default:
                responsePromise = Q.resolve(new UnknownMessageTypeResponse(message.type));
        }

        responsePromise.then(function(hasReply, replyMessage) {
            if (hasReply) {
                var outgoingBuffer = MessageProcessor.toBuffer(replyMessage);
                server.send(outgoingBuffer, 0, outgoingBuffer.length, remote.port, remote.address);
            }
        });
    }

    init();

    // Public API
    return {
        stop: function() {
            return Utility.eventToPromise(server.close);
        }
    };
}

module.exports = UDPBridge;
