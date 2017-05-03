app.filter('safeUser', function () {
    return function (input) {
        if (input)
            return input;
        return "unknown user";
    };
});

app.controller('AuthController', ['$scope', '$location', '$http', '$cookies',
    function ($scope, $location, $http, $cookies) {
        $scope.currentUser = $cookies.get('user') || null;

        $scope.tempUser = "user";

        $scope.lastError = "";
        $scope.inError = $scope.lastError === "";

        $scope.getCurrentView = function () {
            if ($scope.currentUser)
                return "mainpage.htm";
            return "login.htm";
        }

        $scope.submitLogin = function () {
            console.log("try to log", $scope.tempUser)
            $http.post('/login', $scope.tempUser)
                .success(function () {
                    $scope.currentUser = $scope.tempUser;
                })
                .error(function (data) {
                    $scope.lastError = data.loginError;
                });
        };
    }
]);