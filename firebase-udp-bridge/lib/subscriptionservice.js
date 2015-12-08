'use strict';

var SubscriptionEventType = require('./lookups/subscriptioneventtype.js'),
    ErrorMessage = require('./types/errormessage.js'),
    ErrorCode = require('./lookups/errorcode.js');

/**
 * Handles all subscription requests to watch for data changes
 * @param {Firebase} rootFirebase the root node
 * @constructor
 */
function SubscriptionService(rootFirebase) {

    return {
        /**
         * Subscribe to a node
         * @param message the subscription request
         * @param client the request client
         * @returns {Promise} resolves to a response message
         */
        subscribe: function(message, client) {

            /*
            var target = rootFirebase.child(message.path);

            var firebaseEvent;
            switch(message.eventType) {
                case SubscriptionEventType.CHANGED:
                    firebaseEvent = 'value';
                    break;
                default:
                    return new ErrorMessage(ErrorCode.INVALID_SUBSCRIPTION_EVENT_TYPE, message.eventType);
            }

            target.on(firebaseEvent, function(snapshot) {

            });

            */
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
