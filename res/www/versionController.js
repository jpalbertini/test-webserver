app.controller('VersionController', ['$scope', '$http', function ($scope, $http) {
    $http({
        method: 'GET',
        url: '/version'
    }).then(function successCallback(response) {
        $scope.versionInformations = response.data
    }, function errorCallback(response) {
        $scope.versionInformations = response.statusText;
    });
}]);
