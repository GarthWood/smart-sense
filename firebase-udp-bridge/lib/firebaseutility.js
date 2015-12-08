'use strict';

var q = require('Q');

function FirebaseUtility() {

    return {
        /**
         * Sets the value at a node
         * @param {Firebase} target the firebase node
         * @param {*} value the value to save on the target
         * @returns {Promise}
         */
        set: function(target, value) {
            var deferred = q.defer();

            target.set(value, function complete(error) {
                if (error) {
                    deferred.reject(error);
                } else {
                    deferred.resolve();
                }
            });

            return deferred.promise;
        },
        get: function(target) {
            var deferred = q.defer();

            target.once
        }
    }
}

module.exports = new FirebaseUtility();

var deferred = q.defer();

rootFirebase.child(message.path).set(message.value, function complete(error) {
    deferred.resolve();
});

return deferred.promise;
