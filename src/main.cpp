#include <QCoreApplication>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QThread>

#include "webserver.h"
#include "ressourceaccess.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    WebServer wServer(&app);
    RessourceAccess rAccess(&app);
    QWebSocketServer wsServer("TestServer", QWebSocketServer::NonSecureMode, &app);
    QObject::connect(&wsServer, &QWebSocketServer::newConnection, [&](){
       while(wsServer.hasPendingConnections())
       {
           QWebSocket* conn = wsServer.nextPendingConnection();
           qDebug() << "connected " << conn;
       }
    });

    qDebug() << wsServer.listen(QHostAddress::Any, 5000);


    wServer.addDataService("data", [&]() {
        return rAccess.getData();
    });
    wServer.addDataService("version", [&]() {
        return QVariantMap{
            { "version", "1.2.3"},
            { "ver", "1.2.3.4.5"},
            { "version_long", "1.2"},
            { "version_lon_long", "1.2.3.4.5"}
        };
    });
    wServer.addConstant("PORT", "5000");
    wServer.addDataService("testConnection", [&]() {
        QWebSocket wSocket;
        wSocket.open(QUrl("ws://localhost:5000"));
//        do
//        {
//            QThread::msleep(500);
//        } while( wSocket.state() == QAbstractSocket::ConnectingState);
        return QVariantMap{
            { "Status", wSocket.state() == QAbstractSocket::ConnectedState ? "Connected" : "Not Connected"},
                { "State", wSocket.state() }
            };
    });

    return app.exec();
}
