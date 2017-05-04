app.controller('AuthController', ['$scope', '$http', '$cookies', '$mdToast',
    function ($scope, $http, $cookies, $mdToast) {
        $scope.currentUser = $cookies.get('$COOKIE_USERNAME') || null;

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
                    $cookies.put('$COOKIE_USERNAME', user);
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
        $scope.disconnectUser = function () {
            $scope.currentUser = null;
            $cookies.remove('$COOKIE_USERNAME');
        };
    }
]);
