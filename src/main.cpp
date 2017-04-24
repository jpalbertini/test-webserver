#include <QCoreApplication>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QThread>
#include <QAbstractEventDispatcher>
#include <QHostInfo>
#include <QtConcurrent/QtConcurrent>

#include "webserver.h"
#include "ressourceaccess.h"

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
    RessourceAccess rAccess(&app);
    QWebSocketServer wsServer("TestServer", QWebSocketServer::NonSecureMode, &app);
    qDebug() << "Listen: " << wsServer.listen(QHostAddress::Any, 5000);


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
    wServer.addConstant("HOST", QHostInfo::localHostName());
    wServer.addConstant("PORT", "5000");
    wServer.addDataService("testAliveConnection", [&]() {

        QFuture<bool> futur = QtConcurrent::run(&tryConnection, QString("ws://localhost:5000"));
        futur.waitForFinished();

        return QVariantMap{
            { "status", futur.result() ? "check" : "error"}
        };
    });

    return app.exec();
}
