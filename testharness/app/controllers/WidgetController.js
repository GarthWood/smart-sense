'use strict';

app.controller('WidgetController', function($scope, $interval, WidgetService) {

    var lastSpeed = 0;
    var lastSentSpeed = 0;
    var widgetId = 'box0001';

    $scope.slider = {
        minValue: 0,
        options: {
            floor: 0,
            ceil: 10,
            step: 0.05,
            precision: 2,
            draggableRange: false,
            showSelectionBar: false,
            hideLimitLabels: true,
            readOnly: false,
            disabled: false,
            showTicks: false,
            showTicksValues: false
        }
    };

    $scope.speed = 0;
    $scope.rpm = 0;

    /**
     * A callback which sends off data periodically
     */
    var onSendInterval = function() {

        if (lastSentSpeed !== $scope.speed) {
            WidgetService.setData(widgetId, Lookups.Firebase.Node.InputEvents, $scope.speed);
            lastSentSpeed = $scope.speed;
        }
    };

    /**
     * Notification of new data added
     * @param {DataSnapshot} snapshot The new data
     */
    var onDataUpdated = function(snapshot) {
        $scope.rpm = parseFloat(snapshot.val());
    };

    /**
     * Initialises the controller
     */
    var initialise = function() {

        WidgetService.connect()
            .then(function success() {
                WidgetService.registerDataListener(widgetId, Lookups.Firebase.Node.OutputEvents, onDataUpdated);
            })
            .then(function success() {
                WidgetService.registerDisconnectHandler(widgetId, Lookups.Firebase.Node.InputEvents, 0);
            })
            .then(function() {
                $interval(onSendInterval, 1000);
            });
    };

    /**
     * Triggered when the start or stop button is pressed
     * @param {String} event Start or stop event
     */
    $scope.onStartStop = function(event) {

        if ((event === 'start') && (lastSpeed === 0)) {
            lastSpeed = 2;
        }

        $scope.speed = (event === 'start') ? lastSpeed : 0;
    };

    // register slider events
    $scope.$on("slideEnded", function() {
        lastSpeed = $scope.speed;
    });

    // initialise the controller
    initialise();
});
