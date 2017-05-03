#include <QCoreApplication>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QThread>
#include <QAbstractEventDispatcher>
#include <QHostInfo>
#include <QtConcurrent/QtConcurrent>

#include "webserver.h"
#include "ressourceaccess.h"

#define WS_PORT 5000

bool tryConnection(const QString& url)
{
    QWebSocket wSocket;
    wSocket.open(QUrl(url));
    do
    {
        QThread::currentThread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
        QThread::msleep(100);
        qDebug() << "waiting";
    }while(wSocket.state() == QAbstractSocket::ConnectingState);

    return wSocket.state() == QAbstractSocket::ConnectedState;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    WebServer wServer(&app);
    wServer.setBasePath("D:/dev/test-webserver/res/www");
    RessourceAccess rAccess(&app);
    QWebSocketServer wsServer("TestServer", QWebSocketServer::NonSecureMode, &app);

    QObject::connect(&wsServer, &QWebSocketServer::newConnection, [&](){
       while(wsServer.hasPendingConnections())
       {
           QWebSocket* pWebSocket = wsServer.nextPendingConnection();
           QObject::connect(pWebSocket, &QWebSocket::textMessageReceived, [&](const QString& message) {
               pWebSocket->sendTextMessage(message);
           });
           QObject::connect(pWebSocket, &QWebSocket::disconnected, [&]() {
               qDebug() << "disconnected...";
           });
       }
    });

    qDebug() << "Listen: " << wsServer.listen(QHostAddress::Any, WS_PORT);

    wServer.addGetDataService("data", [&]() {
        return rAccess.getData();
    });
    wServer.addGetDataService("version", [&]() {
        return QVariantMap{
            { "version", "1.2.3"},
            { "ver", "1.2.3.4.5"},
            { "version_long", "1.2"},
            { "version_lon_long", "1.2.3.4.5"}
        };
    });
    wServer.addConstant("HOST", QHostInfo::localHostName());
    wServer.addConstant("PORT", QString("%0").arg(WS_PORT));
    wServer.addGetDataService("testAliveConnection", [&]() {
        QFuture<bool> futur = QtConcurrent::run(&tryConnection, QString("ws://localhost:5000"));
        futur.waitForFinished();

        return QVariantMap{
            { "status", futur.result() ? "check" : "error"}
        };
    });

    wServer.start(8080);

    return app.exec();
}
