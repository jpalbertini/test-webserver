app.controller('DownloadContentController', ['$scope', '$http', '$mdToast', function ($scope, $http, $mdToast) {
    $http({
        method: 'GET',
        url: '/GetDownloadableObjects'
    }).then(function successCallback(response) {
        $scope.downloadables = response.data["$DOWNLOADABLE_LIST_KEY"];
    }, function errorCallback(response) {
        $scope.versionInformations = response.statusText;
    });

    $scope.downloadItem = function (item) {
        $http({
            method: 'GET',
            url: "/GetDownloadableObject?object=" + item
        }).then(function successCallback(response) {
            var dataBlob = response.data["$DOWNLOADABLE_DATA_KEY"];
            var blob = new Blob([dataBlob], { type: "application/octet-stream;charset=utf-8;" });
            var downloadLink = angular.element('<a></a>');
            downloadLink.attr('href', window.URL.createObjectURL(blob));
            downloadLink.attr('download', item);
            downloadLink[0].click();
        }, function errorCallback(response) {
            $mdToast.show(
                $mdToast.simple()
                    .textContent("Impossible to fetch the data")
                    .hideDelay(3000)
            );
        });
    }

    $scope.data = [{
        'id': 1,
        'title': 'node1',
        'nodes': [
            {
                'id': 11,
                'title': 'node1.1',
                'nodes': [
                    {
                        'id': 111,
                        'title': 'node1.1.1',
                        'nodes': []
                    }
                ]
            },
            {
                'id': 12,
                'title': 'node1.2',
                'nodes': []
            }
        ]
    }, {
        'id': 2,
        'title': 'node2',
        'nodrop': true, // An arbitrary property to check in custom template for nodrop-enabled
        'nodes': [
            {
                'id': 21,
                'title': 'node2.1',
                'nodes': []
            },
            {
                'id': 22,
                'title': 'node2.2',
                'nodes': []
            }
        ]
    }, {
        'id': 3,
        'title': 'node3',
        'nodes': [
            {
                'id': 31,
                'title': 'node3.1',
                'nodes': []
            }
        ]
    }];
}]);
