'use strict';

/**
 * Handles presence tracking for connected clients
 * @param {Firebase} firebase the root firebase node
 * @constructor
 */
function PresenceService(firebase) {

    // Public API
    return {
        /**
         * Handles a ping request
         * @param message the ping request
         * @param client the remote client
         * @returns {Promise} promise that resolves to a response message
         */
        ping: function(message, client) {
        }
    }
}

module.exports = PresenceService;
