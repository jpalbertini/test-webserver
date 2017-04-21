angular.module('MgmtApp', ['ngMaterial'])
    .config(function ($mdThemingProvider) {
        $mdThemingProvider.theme('default')
            .primaryPalette('blue');
    })
    .controller('VersionController', ['$scope', '$http', function ($scope, $http) {
        $http({
            method: 'GET',
            url: '/version'
        }).then(function successCallback(response) {
            $scope.versionInformations = response.data
        }, function errorCallback(response) {
            $scope.versionInformations = response.statusText;
        });
    }])
    .controller('TestConnectionController', ['$scope', '$http', function ($scope, $http) {
        $http({
            method: 'GET',
            url: '/testConnection'
        }).then(function successCallback(response) {
            $scope.data = response.data
        }, function errorCallback(response) {
            $scope.data = response.statusText;
        });
    }]);
