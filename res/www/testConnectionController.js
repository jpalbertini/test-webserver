app.controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {

    $scope.statusReal = "help";
    $scope.errorTextReal = "";

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

    var webSocket = new WebSocket("ws://$HOST:$PORT");
    console.log("connect ?");
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
}]);
