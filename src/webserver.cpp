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

void WebServer::start(quint16 port)
{
    if(server)
        server->close();

    std::vector<std::string> cpp_options = { "listening_ports", QString::number(port).toStdString()};
    server.reset(new CivetServer(cpp_options));
    server->addHandler("", *this);
}

void WebServer::stop()
{
    server->close();
}

void WebServer::addConstant(const QString &constantName, const QString &value)
{
    constants[QString("$%0").arg(constantName)] = value;
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

#if !defined QT_DEBUG
    if(!cache.contains(requestUri) || cache[requestUri].toString().isEmpty())
#endif
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

    QString fileContent = cache[requestUri].toString();
    for(const auto& constant : constants.keys())
        fileContent.replace(constant, constants[constant]);

    qDebug() << "served " << requestUri;

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
    mg_printf(conn, fileContent.toStdString().c_str());
    mg_printf(conn, "\r\n");

    return true;
}

bool WebServer::handlePost(CivetServer*, mg_connection * conn)
{
    const struct mg_request_info *req_info = mg_get_request_info(conn);

    long long dataSize = req_info->content_length;
    char buf[dataSize + 1] = "";

    mg_read(conn, buf, (size_t)dataSize);
    QString data = QString::fromLocal8Bit(buf);

    qDebug() << "post not supported: " << data << " : ";
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

void WebServer::addDataService(const QString& serviceKey, ServiceCallback callback)
{
    services[serviceKey] = callback;
}
