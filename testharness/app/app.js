'use strict';

var app = angular.module('smartsense', ['ngRoute', 'rzModule']);

app.config(['$routeProvider', function($routeProvider) {

  $routeProvider
      .when('/', {
          templateUrl: 'templates/testharness.html',
          controller: 'TestharnessController'
        })
      .otherwise({redirectTo: '/'});
}]);
