app.controller('DownloadContentController', ['$scope', '$http', '$mdToast', function ($scope, $http, $mdToast) {
    $scope.downloadables = [];
    $http({
        method: 'GET',
        url: '/GetDownloadableObjects'
    }).then(function successCallback(response) {
        var itemList = response.data["$DOWNLOADABLE_LIST_KEY"];
        
        for (item in itemList) {
            var itemName = itemList[item];
            if(itemName.includes("/"))
            {
            }
            else {
                var currentObj = {};
                currentObj.id = item;
                currentObj.title = itemList[item];
                currentObj.downloadLink = itemList[item];
                currentObj.progressVisible = false;
            }
            $scope.downloadables.push(currentObj);
        }
    }, function errorCallback(response) {
        $scope.versionInformations = response.statusText;
    });

    $scope.downloadItem = function (item) {
        var itemLink = item.node.downloadLink;
        item.node.progressVisible = true;
        $http({
            method: 'GET',
            url: "/GetDownloadableObject?object=" + itemLink
        }).then(function successCallback(response) {
            
        item.node.progressVisible = false;
            var dataBlob = response.data["$DOWNLOADABLE_DATA_KEY"];
            var blob = new Blob([dataBlob], { type: "application/octet-stream;" });
            var downloadLink = angular.element('<a></a>');
            downloadLink.attr('href', window.URL.createObjectURL(blob));
            downloadLink.attr('download', itemLink);
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
}]);
