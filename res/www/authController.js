app.controller('AuthController', ['$scope', '$http', '$mdToast',
    function ($scope, $http, $mdToast) {
        $http({
            method: 'GET',
            url: '/getUsername'
        }).then(function successCallback(response) {
            if (response.data["user"]) {
                $scope.currentUser = response.data["user"];
                $mdToast.show(
                    $mdToast.simple()
                        .textContent("Logged in as " + $scope.currentUser)
                        .hideDelay(3000)
                );
            }
            else {
                $mdToast.show(
                    $mdToast.simple()
                        .textContent("Please connect User")
                        .hideDelay(3000)
                );
            }
        }, function errorCallback(response) {
            $mdToast.show(
                $mdToast.simple()
                    .textContent("Server error")
                    .hideDelay(3000)
            );
        });

        $scope.getCurrentView = function () {
            if ($scope.currentUser)
                return "mainpage.htm";
            return "login.htm";
        };
        $scope.submitLogin = function (user) {
            $http({
                method: 'POST',
                url: '/login',
                data: angular.toJson({ user: user })
            }).then(function successCallback(response) {
                if (response.data["valid"]) {
                    $scope.currentUser = user;
                }
                else {
                    $mdToast.show(
                        $mdToast.simple()
                            .textContent("Invalid User")
                            .hideDelay(3000)
                    );
                }
            }, function errorCallback(response) {
                $mdToast.show(
                    $mdToast.simple()
                        .textContent("Server error")
                        .hideDelay(3000)
                );
            });
        };
    }
]);
