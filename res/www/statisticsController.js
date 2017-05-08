app.controller('StatisticsController', ['$scope', '$http', '$timeout', function ($scope, $http, $timeout) {

    var loadTime = 1000, //Load the data every second
        errorCount = 0, //Counter for the server errors
        loadPromise; //Pointer to the promise created by the Angular $timout service


    var retreiveStatistics = function () {
        $http({
            method: 'GET',
            url: '/statistics'
        }).then(function successCallback(response) {
            $scope.statistics = response.data;
            nextLoad();
        }, function errorCallback(response) {});
    }

    var cancelNextLoad = function () {
        $timeout.cancel(loadPromise);
    };

    var nextLoad = function (mill) {
        mill = mill || loadTime;

        //Always make sure the last timeout is cleared before starting a new one
        cancelNextLoad();
        loadPromise = $timeout(retreiveStatistics, mill);
    };

    $scope.$on('$destroy', function () {
        cancelNextLoad();
    });

    retreiveStatistics();

}]);
