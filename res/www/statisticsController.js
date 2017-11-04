app.controller('StatisticsController', ['$scope', '$http', '$timeout', function ($scope, $http, $timeout) {

    $scope.statisticLabels = [];
    $scope.statisticColors = {};
    $scope.statistics = {};

    var webSocket = new WebSocket("ws://$HOST:$PORT");
    var connected = false;

    webSocket.onerror = function (e) {
        console.log("no web socket...");
    };

    webSocket.onmessage = function(response) {

        var data = JSON.parse(response.data);

        for (statistic in data) {
            if (!(statistic in $scope.statistics))
            {
                $scope.statistics[statistic] = [];
                var currentColor = allowedColors[colorIndex];
                $scope.statisticColors[statistic] = [ currentColor ];
                colorIndex++;
                //colorIndex %= allowedColors.length;
            }

            $scope.statistics[statistic].push(data[statistic]);
            if ($scope.statistics[statistic].length > records)
                $scope.statistics[statistic].splice(0, 1);
        }

        var date = new Date(Date.now());
        var dateString = date.getHours().toString() + ":" + date.getMinutes().toString() + ":" + date.getSeconds().toString();

        $scope.statisticLabels.push(dateString);
        if ($scope.statisticLabels.length > records)
            $scope.statisticLabels.splice(0, 1);
            
        nextLoad();
    };

    webSocket.onopen = function(e) {
        connected = true;
    };
    
    var loadTime = 1000, //Load the data every second
        errorCount = 0, //Counter for the server errors
        loadPromise, //Pointer to the promise created by the Angular $timout service
        records = 60, // Number of records max per graph 
        colorIndex = 0, //current color index
        allowedColors = ['#97BBCD', '#DCDCDC', '#F7464A', '#46BFBD', '#FDB45C', '#949FB1', '#4D5360']; //Allowed colors

    $scope.zoom = 250;

    var retreiveStatistics = function () { 
        if(!connected)
        {
            nextLoad();
            return;
        }

        var payload = {};
        payload["service"] = "statistics";
        var data = JSON.stringify(payload);
        webSocket.send(data);
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
