app.controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {

    $scope.statusAlive = "help";
    $scope.errorTextAlive = "";
    
    $scope.statusReal = "help";
    $scope.errorTextReal = "";

    $scope.testAliveConnection = function() {
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
    }

    $scope.testRealConnection = function() {
        $scope.errorTextReal = "";
        $scope.statusReal = "refresh";

        var webSocket = new WebSocket("ws://$HOST:$PORT");
        webSocket.onopen = function(e) {
            $scope.statusReal = "check";
            $scope.errorTextReal = "";
            webSocket.close();
        };
        webSocket.onerror = function(e) {
            $scope.statusReal = "error";
            $scope.errorTextReal = e;
        };
    }

}]);
