'use strict';

app.service('WidgetService', function($q) {

    var firebase = new Firebase(Lookups.Firebase.Credentials.Domain);
    var connectionDeferred = $q.defer();

    /**
     * Authenticates with Firebase and resolves the promise accordingly
     */
    var connectInternal = function() {

        firebase.authWithCustomToken(Lookups.Firebase.Credentials.Secret,
            function complete(error) {
                if (!error) {
                    connectionDeferred.resolve();
                } else {
                    connectionDeferred.reject(error);
                }
            });
    };

    /**
     * Notification that a Firebase event was cancelled
     * @param error
     */
    var onCancelled = function(error) {

    };

    /**
     * Returns a promise for the connection state of Firebase
     * @returns {promise}
     */
    this.connect = function() {
        return connectionDeferred.promise;
    };

    /**
     * Adds the widget properties to Firebase
     * @param {String} widgetId The unique ID of the widget
     * @param {String} widgetType The type of widget
     * @returns {promise}
     */
    this.registerWidget = function(widgetId, widgetType) {

        var deferred = $q.defer();

        firebase
            .child(widgetId)
            .child(Lookups.Firebase.Node.Properties)
            .child(Lookups.Firebase.Node.Type)
            .set(widgetType, function(error) {
                if (!error) {
                    deferred.resolve();
                } else {
                    deferred.reject(error);
                }
            });

        return deferred.promise;
    };

    /**
     * Subscribes for data updates on the specific widget
     * @param {String} widgetId The unique ID of the widget
     * @param {String} event The event to listen on
     * @param {Function} callback The function to call when a new data is added
     */
    this.registerDataListener = function(widgetId, event, callback) {

        firebase.child(widgetId)
                .child(event)
                .child(Lookups.Firebase.Node.Value)
                .on(Lookups.Firebase.Event.Value, callback, onCancelled);
    };

    /**
     * Updates data to the Firebase widget on the specified event
     * @param {String} widgetId The widget ID
     * @param {String} event Either the incoming or outgoing event
     * @param {*} value The value of the data
     */
    this.setData = function(widgetId, event, value) {

        var deferred = $q.defer();

        firebase
            .child(widgetId)
            .child(event)
            .child(Lookups.Firebase.Node.Value)
            .set(value, function(error) {
                if (!error) {
                    deferred.resolve();
                } else {
                    deferred.reject(error);
                }
            });

        return deferred.promise;
    };

    /**
     * Registers a disconnect handler for a widget
     * @param {String} widgetId The widget ID
     * @param {String} event Either the incoming or outgoing event
     * @param {*} value The value of the data
     */
    this.registerDisconnectHandler = function(widgetId, event, value) {

        firebase
            .child(widgetId)
            .child(event)
            .child(Lookups.Firebase.Node.Value)
            .onDisconnect()
            .set(value);
    };

    // start the Firebase connection
    connectInternal();
});
