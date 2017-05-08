app = angular.module('MgmtApp', ['ngMaterial', 'ngRoute', 'ngCookies', 'chart.js']);

app.config(function (ChartJsProvider) {
    // Configure all charts
    ChartJsProvider.setOptions({
        animation: false
    });
});