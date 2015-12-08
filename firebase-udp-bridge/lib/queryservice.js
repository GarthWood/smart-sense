'use strict';

var q = require('Q');

/**
 * Handles data reads and writes (not change events)
 * @param {Firebase} rootFirebase the root firebase node
 * @constructor
 */
function QueryService(rootFirebase) {

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
         * Sets a float value at a node
         * @param message the request message
         * @param client the remote client
         * @returns {Promise} resolves to a response message
         */
        setFloat: function(message, client) {

            var deferred = q.defer();

            rootFirebase.child(message.path).set(message.value, function complete(error) {
                deferred.resolve();
            });

            return deferred.promise;
        }
    };
}

module.exports = QueryService;
