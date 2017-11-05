#include <QCoreApplication>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QThread>
#include <QAbstractEventDispatcher>
#include <QHostInfo>
#include <QtConcurrent/QtConcurrent>
#include <QFileInfo>

#include "webserver.hpp"
#include "websocketserver.hpp"
#include "ressourceaccess.hpp"

#define DOWNLOADABLE_BASE_PATH ":/www/"

#define WS_PORT 5000
#define COOKIE_USERNAME "user"
#define DOWNLOADABLE_OBJECT_KEY "object"
#define DOWNLOADABLE_LIST_KEY "list"
#define DOWNLOADABLE_DATA_KEY "data"

#define BASE_DOWNLOAD_DIR "C:/Users/jpalb/Downloads/"

bool tryConnection(const QString& url)
{
    QWebSocket wSocket;
    wSocket.open(QUrl(url));
    do
    {
        QThread* pThread = QThread::currentThread();
        if(pThread)
        {
            QAbstractEventDispatcher* dispatcher = pThread->eventDispatcher();
            if(dispatcher)
                dispatcher->processEvents(QEventLoop::AllEvents);
        }
        QThread::msleep(100);
        qDebug() << "waiting";
    }while(wSocket.state() == QAbstractSocket::ConnectingState);

    return wSocket.state() == QAbstractSocket::ConnectedState;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QMap<QString, QString> usernamesByIPs;

    WebServer wServer(&app);
    wServer.setBasePath("D:/dev/test-webserver/res/www");
    RessourceAccess rAccess(&app);
    WebSocketServer wsServer(&app);
    wsServer.start(WS_PORT);

    wServer.addConstant("HOST", QHostInfo::localHostName());
    wServer.addConstant("PORT", QString("%0").arg(WS_PORT));
    wServer.addConstant("COOKIE_USERNAME", COOKIE_USERNAME);
    wServer.addConstant("DOWNLOADABLE_OBJECT_KEY", DOWNLOADABLE_OBJECT_KEY);
    wServer.addConstant("DOWNLOADABLE_LIST_KEY", DOWNLOADABLE_LIST_KEY);
    wServer.addConstant("DOWNLOADABLE_DATA_KEY", DOWNLOADABLE_DATA_KEY);

    wServer.addGetDataService("data", [&](const ClientInformations&, const GetParameters&) {
        return rAccess.getData();
    });
    wServer.addGetDataService("version", [&](const ClientInformations&, const GetParameters&) {
        return QVariantMap{
            { "version", "1.2.3"},
            { "ver", "1.2.3.4.5"},
            { "version_long", "1.2"},
            { "version_lon_long", "1.2.3.4.5"}
        };
    });

    wServer.addGetDataService("GetDownloadableObjects", [&](const ClientInformations&, const GetParameters&) {
        QVariantMap result;

        auto dirList = QDir(BASE_DOWNLOAD_DIR).entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs );
        for(const auto& dir: dirList)
        {
            QVariantList currentFileList;
            for(const auto& item: QDir(dir.absoluteFilePath()).entryInfoList(QDir::NoDotAndDotDot | QDir::Files ))
                currentFileList += item.fileName();

            result[dir.fileName()] = currentFileList;
        }

        qDebug() << result;
        return QVariantMap {
            { DOWNLOADABLE_LIST_KEY, result }
        };
    });
    wServer.addGetDataService("GetDownloadableObject", [&](const ClientInformations&, const GetParameters& parameters) {
        QString path = QString(BASE_DOWNLOAD_DIR) + parameters[DOWNLOADABLE_OBJECT_KEY];

        QThread::sleep(5);

        QByteArray data;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
            data = file.readAll();

        return QVariantMap {
            { DOWNLOADABLE_DATA_KEY, data }
        };
    });
    wServer.addGetDataService("testAliveConnection", [&](const ClientInformations&, const GetParameters&) {
        QFuture<bool> futur = QtConcurrent::run(&tryConnection, QString("ws://localhost:5000"));
        futur.waitForFinished();

        return QVariantMap{ { "status", futur.result() ? "check" : "error"} };
    });


    wServer.addGetDataService("getUsername", [&](const ClientInformations& client, const GetParameters&) {
        return QVariantMap{ { "user", usernamesByIPs[client.ipAddress] } };
    });

    wServer.addPostDataService("login", [&](const ClientInformations& client, const QVariantMap& input) {
        usernamesByIPs[client.ipAddress] = input["user"].toString();
        return QVariantMap{
            { "valid", true}
        };
    });

    wServer.start(8080);

    return app.exec();
}
