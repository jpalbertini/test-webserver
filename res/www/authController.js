app.filter('safeUser', function () {
    return function (input) {
        if (input)
            return input;
        return "unknown user";
    };
});

app.controller('AuthController', ['$scope', '$location', '$http', '$cookies', '$mdToast',
    function ($scope, $location, $http, $cookies, $mdToast) {
        $scope.currentUser = $cookies.get('user') || null;

        $scope.getCurrentView = function () {
            if ($scope.currentUser)
                return "mainpage.htm";
            return "login.htm";
        }

        $scope.submitLogin = function (user) {
            $http({
                method: 'POST',
                url: '/login',
                data: angular.toJson({ user: user })
            }).then(function successCallback(response) {
                if (response.data["valid"] === 1) {
                    $scope.currentUser = user;
                    $cookies.put('user', user)
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