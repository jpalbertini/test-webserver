#include "webserver.h"

#include <QDebug>
#include <QFile>

webserver::webserver(QObject *parent) : QObject(parent)
{
}

bool webserver::handleGet(CivetServer *, mg_connection *conn)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    qDebug()  << req_info->request_uri;

    QString filePath = QString::fromLocal8Bit(req_info->request_uri);
    filePath.remove(0, 1);

    if(filePath.isEmpty())
        filePath = "index.html";

    filePath = ":/" + filePath;


    if(!cache.contains(filePath) || cache[filePath].toString().isEmpty())
    {
        QString content;
        QFile mainF(filePath);
        if(mainF.open(QIODevice::ReadOnly))
            content = QString::fromLatin1(mainF.readAll());
        else
        {
            qWarning() << "cannot open file " << filePath;
            return false;
        }
        mainF.close();
        cache[filePath] = content;
    }

    QString file = cache[filePath].toString();

    qDebug() << "served " << filePath;

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
    mg_printf(conn, file.toStdString().c_str());
    mg_printf(conn, "\r\n");

    return true;
}

bool webserver::handlePost(CivetServer*, mg_connection *)
{
    qDebug() << "post not supported";
    return false;
}

bool webserver::handleHead(CivetServer *, mg_connection *)
{
    qDebug() << "head not supported";
    return false;
}

bool webserver::handlePut(CivetServer *, mg_connection *)
{
    qDebug() << "put not supported";
    return false;
}

bool webserver::handleDelete(CivetServer *, mg_connection *)
{
    qDebug() << "delete not supported";
    return false;
}

bool webserver::handleOptions(CivetServer *, mg_connection *)
{
    qDebug() << "options not supported";
    return false;
}

bool webserver::handlePatch(CivetServer *, mg_connection *)
{
    qDebug() << "patch not supported";
    return false;
}
