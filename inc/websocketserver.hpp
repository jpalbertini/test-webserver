#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(QObject *parent = nullptr);

    void start(int port);
    void parseAndReply(QObject* sender, QByteArray data);

public slots:
    void onNewConnection();
    void onBinaryMessage(const QByteArray& data);
    void onTextMessage(const QString& data);

private:
    QWebSocketServer* m_pServer;
};

#endif // WEBSOCKETSERVER_H
