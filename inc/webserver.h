#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QVariantMap>

#include <functional>
#include <memory>

#include "CivetServer.h"

using ServiceCallback = std::function<QVariantMap()>;

class WebServer : public QObject, public CivetHandler
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);
    void setBasePath(const QString& path);

    void start(quint16 port);
    void stop();

    void addConstant(const QString& constantName, const QString& value);
    void addDataService(const QString& serviceKey, ServiceCallback callback);

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
    QMap<QString, ServiceCallback> services;
    QMap<QString, QString> constants;
};

#endif // WEBSERVER_H
