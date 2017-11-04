#include "websocketserver.hpp"

#include <QWebSocket>

#include <QJsonDocument>
#include <QJsonObject>

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent)
{
    m_pServer = new QWebSocketServer("local", QWebSocketServer::NonSecureMode, this);
    connect(m_pServer, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
}

void WebSocketServer::start(int port)
{
    m_pServer->listen(QHostAddress::Any, port);
}

void WebSocketServer::parseAndReply(QObject *sender, QByteArray data)
{
    auto socket = qobject_cast<QWebSocket*>(sender);
    if(!socket)
    {
        qWarning() << "sender is not a socket";
        return;
    }

    auto dataMap = QJsonDocument::fromJson(data).object().toVariantMap();
    auto service = dataMap["service"].toString();
    qDebug() << "received " << data << " : " << service;

    if(service == "statistics")
    {
        srand(QTime::currentTime().msec());
        QVariantMap data;
        data["A"] = rand() % 100;
        data["B"] = rand() % 100;
        data["C"] = rand() % 100;
        data["D"] = rand() % 100;
        data["E"] = rand() % 100;
        data["F"] = rand() % 100;
        data["G"] = rand() % 100;

        QByteArray payload = QJsonDocument::fromVariant(data).toJson();
        QString str = QString::fromUtf8(payload);
        socket->sendTextMessage(str);
        qDebug() << "send str " << str;
    }
}

void WebSocketServer::onNewConnection()
{
    while(m_pServer->hasPendingConnections())
    {
        QWebSocket* socket = m_pServer->nextPendingConnection();
        qDebug() << "incoming: " << socket->peerAddress().toString();
        connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessage);
        connect(socket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::onBinaryMessage);
    }
}

void WebSocketServer::onBinaryMessage(const QByteArray &data)
{
    parseAndReply(sender(), data);
}

void WebSocketServer::onTextMessage(const QString &data)
{
    parseAndReply(sender(), data.toUtf8());
}
