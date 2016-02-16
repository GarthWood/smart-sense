'use strict';

var ProtoBuf = require('protobufjs'),
    path = require('path');

/**
 * Provides builders for the UDP wire protocol
 * @constructor
 */
function ProtocolProvider() {

    var builder = ProtoBuf.loadProtoFile('./../protocols/messages.proto');

    return {
        createServiceMessage: function() {
            return builder.build('ServiceMessage');
        }
    };
}

module.exports = new ProtocolProvider();
