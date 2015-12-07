'use strict';

app.controller('BoxController', function($scope, $interval, WidgetService) {

    var widgetId = 'box0001';
    var widgetType = 'motor';

    var finalSpeed = 0;
    var lastSentRpm = 0;

    $scope.speed = 0;
    $scope.angle = 0;
    $scope.rpm = 0;

    var spinStartTime = 0;

    /**
     * Notification that a revolution has completed
     */
    var onRevolutionComplete = function(rpm) {
        $scope.rpm = rpm.toFixed(2);
    };

    /**
     * Notification of new data added
     * @param {DataSnapshot} snapshot The new data
     */
    var onDataUpdated = function(snapshot) {
        finalSpeed = parseFloat(snapshot.val());
    };

    /**
     * A callback which sends off data periodically
     */
    var onSendInterval = function() {

        if (parseInt(lastSentRpm) !== parseInt($scope.rpm)) {
            WidgetService.setData(widgetId, Lookups.Firebase.Node.OutputEvents, $scope.rpm);
            lastSentRpm = $scope.rpm;
        }
    };

    /**
     * Initialises the controller
     */
    var initialise = function() {

        WidgetService.connect()
            .then(function success() {
                WidgetService.registerWidget(widgetId, widgetType);
            })
            .then(function success() {
                WidgetService.registerDataListener(widgetId, Lookups.Firebase.Node.InputEvents, onDataUpdated);
            })
            .then(function success() {
                WidgetService.registerDisconnectHandler(widgetId, Lookups.Firebase.Node.OutputEvents, 0);
            })
            .then(startSpin)
            .then(function success() {
                $interval(onSendInterval, 1000);
            });
    };

    /**
     * Start the spin timer
     */
    var startSpin = function() {

        spinStartTime = (new Date).getTime();

        $interval(function() {

            if ($scope.speed < finalSpeed) {
                $scope.speed = Math.min($scope.speed += 0.005, finalSpeed);
            } else if ($scope.speed > finalSpeed) {
                $scope.speed = Math.max($scope.speed -= 0.005, 0);
            }

            $scope.angle += $scope.speed;

            if ($scope.angle >= 360) {

                var now = (new Date).getTime();
                $scope.angle -= 360;
                onRevolutionComplete(60 / ((now - spinStartTime) / 1000));
                spinStartTime = now;
            }

            if ($scope.speed === 0) {
                onRevolutionComplete(0);
            }
        }, 5);
    };

    // initialise the controller
    initialise();
});
