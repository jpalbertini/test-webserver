#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QVariantMap>

#include <functional>
#include <memory>

#include "CivetServer.h"

using GetParameters = QMap<QString, QString>;
using GetDataCallback = std::function<QVariantMap(const QString&, const GetParameters&)>;
using PostDataCallback = std::function<QVariantMap(const QString&, const QVariantMap&)>;

class WebServer : public QObject, public CivetHandler
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);
    void setBasePath(const QString& path);

    void start(quint16 port);
    void stop();

    void addConstant(const QString& constantName, const QString& value);
    void addGetDataService(const QString& serviceKey, GetDataCallback callback);
    void addPostDataService(const QString& serviceKey, PostDataCallback callback);

    // CivetHandler interface
public:
    bool handleGet(CivetServer *server, mg_connection *conn);
    bool handlePost(CivetServer *server, mg_connection *conn);
    bool handleHead(CivetServer *server, mg_connection *conn);
    bool handlePut(CivetServer *server, mg_connection *conn);
    bool handleDelete(CivetServer *server, mg_connection *conn);
    bool handleOptions(CivetServer *server, mg_connection *conn);
    bool handlePatch(CivetServer *server, mg_connection *conn);

protected:
    std::unique_ptr<CivetServer> server;
    QString basePath = ":/www";
    QVariantMap cache;
    QMap<QString, GetDataCallback> getDataServices;
    QMap<QString, PostDataCallback> postDataServices;
    QMap<QString, QString> constants;
};

#endif // WEBSERVER_H
