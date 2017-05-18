app.controller('DownloadContentController', ['$scope', '$http', '$mdToast', '$mdSidenav', function ($scope, $http, $mdToast, $mdSidenav) {
    $scope.downloadables = [];
    $http({
        method: 'GET',
        url: '/GetDownloadableObjects'
    }).then(function successCallback(response) {
        var itemMap = response.data["$DOWNLOADABLE_LIST_KEY"];
        var idCpt = 0;
        for (key in itemMap) {

            var currentNode = {};
            currentNode.id = idCpt++;
            currentNode.title = key;
            currentNode.nodes = [];
            var itemList = itemMap[key];
            for (item in itemList) {
                var currentObj = {};
                currentObj.id = idCpt++;
                currentObj.title = itemList[item];
                currentObj.downloadLink = key + "/" + itemList[item];
                currentObj.downloadLink.replace(" ", "&nbsp")
                currentObj.progressVisible = false;
                currentNode.nodes.push(currentObj);
            }
            $scope.downloadables.push(currentNode);
        }
    }, function errorCallback(response) {
        $scope.versionInformations = response.statusText;
    });

    $scope.downloadItem = function (item) {
        item.node.progressVisible = true;
        $http({
            method: 'GET',
            url: "/GetDownloadableObject?object=" + item.node.downloadLink
        }).then(function successCallback(response) {

            item.node.progressVisible = false;
            var dataBlob = response.data["$DOWNLOADABLE_DATA_KEY"];
            var blob = new Blob([dataBlob], {
                type: "application/octet-stream;"
            });
            var downloadLink = angular.element('<a></a>');
            downloadLink.attr('href', window.URL.createObjectURL(blob));
            downloadLink.attr('download', item.node.title);
            downloadLink[0].click();
        }, function errorCallback(response) {
            item.node.progressVisible = false;
            $mdToast.show(
                $mdToast.simple()
                .textContent("Impossible to fetch the data")
                .hideDelay(3000)
            );
        });
    };

    $scope.previewItem = function (item) {
        item.node.progressVisible = true;
        $http({
            method: 'GET',
            url: "/GetDownloadableObject?object=" + item.node.downloadLink
        }).then(function successCallback(response) {
            item.node.progressVisible = false;
            $scope.currentFileName = item.node.title;
            var text = response.data["$DOWNLOADABLE_DATA_KEY"];
            $scope.currentFileContent = response.data["$DOWNLOADABLE_DATA_KEY"];
            $mdSidenav('right').toggle();
        }, function errorCallback(response) {
            item.node.progressVisible = false;
            $mdToast.show(
                $mdToast.simple()
                .textContent("Impossible to fetch the data")
                .hideDelay(3000)
            );
        });
    };

    $scope.close = function () {
      $mdSidenav('right').close();
    };
}]);
