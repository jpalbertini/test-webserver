#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QVariantMap>

#include "CivetServer.h"

class webserver : public QObject, public CivetHandler
{
    Q_OBJECT
public:
    explicit webserver(QObject *parent = 0);

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
    QVariantMap cache;
};

#endif // WEBSERVER_H
