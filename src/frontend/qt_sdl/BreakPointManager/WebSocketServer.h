#pragma once

#include <QObject>

class QString;
class QImage;

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    WebSocketServer(int port, QObject* parent = nullptr);
    ~WebSocketServer();

    void sendMessage(const QString& message, const QImage& image);

private slots:
  void onNewConnection();
  void onSocketDisconnected();

private:
  QWebSocketServer* server;
  QList<QWebSocket*> clients;
};
