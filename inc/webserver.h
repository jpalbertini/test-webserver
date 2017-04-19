#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QVariantMap>

#include <functional>

#include "CivetServer.h"

using ServiceCallback = std::function<QVariantMap()>;

class WebServer : public QObject, public CivetHandler
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = 0);
    void setBasePath(const QString& path);
    void registerDataService(const QString& serviceKey, ServiceCallback callback);

signals:

public slots:

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
    QString basePath = ":";
    QVariantMap cache;
    QMap<QString, ServiceCallback> services;
};

#endif // WEBSERVER_H
