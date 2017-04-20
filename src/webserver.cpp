#include "webserver.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

WebServer::WebServer(QObject *parent)
    : QObject(parent)
    , CivetHandler()
{
    std::vector<std::string> cpp_options = { "listening_ports", "8080"};
    server.reset(new CivetServer(cpp_options));

    server->addHandler("", *this);
}

void WebServer::setBasePath(const QString &path)
{
    basePath = path;
}

bool WebServer::handleGet(CivetServer *, mg_connection *conn)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    qDebug()  << req_info->request_uri;

    QString requestUri = QString::fromLocal8Bit(req_info->request_uri);
    requestUri.remove(0, 1);

    if(services.contains(requestUri))
    {
        auto data = services[requestUri]();
        QJsonDocument doc = QJsonDocument(QJsonObject::fromVariantMap(data));
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/jsonl\r\nConnection: close\r\n\r\n");
        mg_printf(conn, doc.toJson().data());
        mg_printf(conn, "\r\n");

        qDebug() << "served service data " << requestUri;

        return true;
    }

    if(requestUri.isEmpty())
        requestUri = "index.html";
    requestUri = basePath + "/" + requestUri;


    if(!cache.contains(requestUri) || cache[requestUri].toString().isEmpty())
    {
        QString content;
        QFile mainF(requestUri);
        if(mainF.open(QIODevice::ReadOnly))
            content = QString::fromLatin1(mainF.readAll());
        else
        {
            qWarning() << "cannot open file " << requestUri;
            return false;
        }
        mainF.close();
        cache[requestUri] = content;
    }

    QString file = cache[requestUri].toString();

    qDebug() << "served " << requestUri;

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
    mg_printf(conn, file.toStdString().c_str());
    mg_printf(conn, "\r\n");

    return true;
}

bool WebServer::handlePost(CivetServer*, mg_connection *)
{
    qDebug() << "post not supported";
    return false;
}

bool WebServer::handleHead(CivetServer *, mg_connection *)
{
    qDebug() << "head not supported";
    return false;
}

bool WebServer::handlePut(CivetServer *, mg_connection *)
{
    qDebug() << "put not supported";
    return false;
}

bool WebServer::handleDelete(CivetServer *, mg_connection *)
{
    qDebug() << "delete not supported";
    return false;
}

bool WebServer::handleOptions(CivetServer *, mg_connection *)
{
    qDebug() << "options not supported";
    return false;
}

bool WebServer::handlePatch(CivetServer *, mg_connection *)
{
    qDebug() << "patch not supported";
    return false;
}

void WebServer::registerDataService(const QString& serviceKey, ServiceCallback callback)
{
    services[serviceKey] = callback;
}
