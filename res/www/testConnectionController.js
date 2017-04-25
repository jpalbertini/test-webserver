app.controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {

    $scope.statusAlive = "help";
    $scope.errorTextAlive = "";

    $scope.statusReal = "help";
    $scope.errorTextReal = "";

    $scope.testAliveConnection = function () {
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
        
    var webSocket = new WebSocket("ws://$HOST:$PORT");
    $scope.testRealConnection = function () {
        $scope.errorTextReal = "";
        $scope.statusReal = "refresh";
    
        webSocket.onopen = function (e) {
            console.log("open");
        };
        webSocket.onerror = function (e) {
            $scope.statusReal = "error";
            $scope.errorTextReal = e;
            console.log("error");
        };
        webSocket.onmessage = function (event) {
            $scope.statusReal = "check";
            $scope.errorTextReal = "";
            console.log("message");
        };
        webSocket.send("{ \"verb\" : \"version\"}");
    };
}]);
