app.controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {

    $scope.statusReal = "help";
    $scope.statusAlive = "refresh";

    $scope.checkAliveConnection = function () {
        $scope.errorTextAlive = "";
        $scope.statusAlive = "refresh";
        $http({
            method: 'GET',
            url: '/testAliveConnection'
        }).then(function successCallback(response) {
            $scope.statusAlive = response.data["status"];
        }, function errorCallback(response) {
            $scope.statusAlive = "error";
        });
    };

    $scope.checkRealConnection = function () {
        var webSocket = new WebSocket("ws://$HOST:$PORT");

        $scope.statusReal = "help";
        $scope.errorTextReal = "";

        webSocket.onopen = function (e) {
            $scope.statusReal = "check";
        };
        webSocket.onerror = function (e) {
            $scope.statusReal = "error";
        };
    };

    $scope.checkConnections = function () {
        $scope.checkAliveConnection();
        $scope.checkRealConnection();
    };

    $scope.checkConnections();

}]);
