app.get('*', function (req, res) {
    res.render('index.html', {
        user: req.user ? JSON.stringify(req.user.username) : 'null'
    });
});

app.service('Auth', function () {
    var user = window.user;
    return {
        getUser: function () {
            return user;
        },
        setUser: function (newUser) {
            user = newUser;
        },
        isConnected: function () {
            return !!user;
        }
    };
});

app.controller('AuthController', ['$scope', '$location', '$http', 'Auth',
    function ($scope, $location, $http, Auth) {
        $scope.userData = {};
        $scope.loginError = '';

        $scope.submitLogin = function () {
            $http.post('/login', this.userData)
                .success(function (user) {
                    Auth.setUser(user);
                    $location.url('/');
                })
                .error(function (data) {
                    $scope.loginError = data.loginError;
                });
        };

        var checkIsConnected = function ($q, $timeout, $http, $location) {
            var deferred = $q.defer();

            $http.get('/loggedin').success(function (user) {
                if (user !== '-1') {
                    $timeout(deferred.resolve, 0);
                } else {
                    $timeout(deferred.reject, 0);
                    $location.url('/login');
                }
            });

            return deferred.promise;
        };
    }
]);