#pragma once

#include <QObject>
#include <QString>

class QImage;
class EmuThread;

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
  WebSocketServer(int port, QObject* parent = nullptr);
  ~WebSocketServer();

  void sendMessage(const QString& message, const QImage& image);
  void setEmuThread(EmuThread* inEmuThread);
  void sendUpdatedScreenshot(const QImage& image);

signals:
  void onNewScreenshot();

private slots:
  void onNewConnection();
  void onSocketDisconnected();
  void onSocketMessageReceived(QString message);

private:
  EmuThread* emuThread = nullptr;
  QWebSocketServer* server;
  QList<QWebSocket*> clients;

  QString createInitialMessage() const;
};
