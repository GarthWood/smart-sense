'use strict';

/**
 * Handles data reads and writes (not change events)
 * @param {Firebase} firebase the root firebase node
 * @constructor
 */
function QueryService(firebase) {

    // Public API
    return {
        /**
         * Gets a value at a node
         * @param message the request message
         * @param client the remote client
         * @returns {Promise} resolves to a response message
         */
        get: function(message, client) {
        },
        /**
         * Sets a value at a node
         * @param message the request message
         * @param client the remote client
         * @returns {Promise} resolves to a response message
         */
        set: function(message, client) {
        }
    };
}

module.exports = QueryService;
