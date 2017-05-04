app.controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {

    $scope.statusReal = "help";
    $scope.errorTextReal = "";

    $scope.errorTextAlive = "";
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
            $scope.errorTextAlive = response.statusText;
        });
    };

    $scope.checkRealConnection = function () {
        var webSocket = new WebSocket("ws://$HOST:$PORT");

        $scope.statusReal = "help";
        $scope.errorTextReal = "";

        webSocket.onopen = function (e) {
            console.log("open");
            $scope.statusReal = "check";
            $scope.errorTextReal = "";
        };
        webSocket.onerror = function (e) {
            console.log("error", e);
            $scope.statusReal = "error";
            $scope.errorTextAlive = response.statusText;
        };
    };

    $scope.checkConnections = function () {
        $scope.checkAliveConnection();
        $scope.checkRealConnection();
    };

    $scope.checkConnections();

}]);
