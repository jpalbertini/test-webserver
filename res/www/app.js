app = angular.module('MgmtApp', ['ngMaterial', 'ngRoute', 'ngCookies', 'chart.js', 'ui.tree']);

app.config(function (ChartJsProvider) {
    // Configure all charts
    ChartJsProvider.setOptions({
        animation: false
    });
});