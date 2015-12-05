/**
 * Handles all subscription requests to watch for data changes
 * @param {Firebase} firebase the root node
 * @constructor
 */
function SubscriptionService(firebase) {

    return {
        /**
         * Subscribe to a node
         * @param message the subscription request
         * @param client the request client
         * @returns {Promise} resolves to a response message
         */
        subscribe: function(message, client) {
        },
        /**
         * Unsubscribe from a node
         * @param message the unsubscribe request
         * @param client the request client
         * @returns {Promise} resolves to a response message
         */
        unsubscribe: function(message, client) {
        }
    };
}

module.exports = SubscriptionService;
