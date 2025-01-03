#include "WebSocketServer.h"

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QBuffer>
#include "EmuThread.h"
#include "NDS.h"
#include "NDSCart.h"
#include "NDS_Header.h"

namespace {
  QString convertQImageToBase64(const QImage &image) {
    if (image.isNull()) {
        qWarning() << "Image is null, cannot convert to Base64.";
        return QString();
    }

    // Encode QImage as PNG into a QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "PNG")) {
        qWarning() << "Failed to encode QImage as PNG.";
        return QString();
    }

    // Convert QByteArray to Base64
    QString base64String = QString("data:image/png;base64,%1").arg(QString{byteArray.toBase64()});
    return base64String;
}
}

WebSocketServer::WebSocketServer(int port, QObject* parent)
  : QObject(parent)
  , server{new QWebSocketServer(QString{"texthook-server"}, QWebSocketServer::NonSecureMode, this)}
{
  qDebug() << "WebSocketServer created at address:" << this;
  
  if (server->listen(QHostAddress::Any, port))
  {
    qDebug() << "WebSocket server listening on port" << port;
    connect(server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
  }
}

WebSocketServer::~WebSocketServer()
{
  server->close();
  qDeleteAll(clients.begin(), clients.end());
}

void WebSocketServer::onNewConnection()
{
  QWebSocket* socket = server->nextPendingConnection();
  connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::onSocketDisconnected);
  connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onSocketMessageReceived);

  clients.append(socket);

  QString initialMessage = createInitialMessage();
  socket->sendTextMessage(initialMessage);

  qDebug() << "New client: " << socket->peerAddress().toString();
  qDebug() << "initial message: " << initialMessage;
}
  
void WebSocketServer::onSocketDisconnected()
{
  qDebug() << "client disconnected";
  QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
  if (socket)
  {
    clients.removeAll(socket);
    socket->deleteLater();
  }
}


void WebSocketServer::sendMessage(const QString& message, const QImage& image)
{
  QJsonObject jsonObject;
  jsonObject["message"] = message;
  if (!image.isNull())
  {
    jsonObject["image"] = convertQImageToBase64(image);
  }

  QJsonDocument jsonDocument(jsonObject);
  QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

  for (QWebSocket* socket : clients)
  {
    socket->sendTextMessage(jsonString);
  }

  qDebug() << "Sent message to clients" << this;
}

QString WebSocketServer::createInitialMessage() const
{
  QJsonObject jsonObject;
  jsonObject["method"] = "initialize";
  jsonObject["title"] = "suikaDS";

  if (emuThread != nullptr) { 
    melonDS::NDSCart::CartCommon* cart = emuThread->NDS->NDSCartSlot.GetCart();
    if (cart)
    {
      const melonDS::NDSHeader& header = cart->GetHeader();
      jsonObject["title"] = QString::fromLatin1(header.GameTitle, 12);
    }
  }
  QJsonDocument jsonDocument(jsonObject);
  return jsonDocument.toJson(QJsonDocument::Compact);
}

void WebSocketServer::setEmuThread(EmuThread* inEmuThread)
{
  emuThread = inEmuThread;
}

void WebSocketServer::onSocketMessageReceived(const QString message)
{
  if (message == "update_screenshot") {
    emit onNewScreenshot();
  }
}

void WebSocketServer::sendUpdatedScreenshot(const QImage& image)
{
  QJsonObject jsonObject;
  jsonObject["method"] = "update_screenshot";
  jsonObject["image"] = convertQImageToBase64(image);
  QJsonDocument jsonDocument(jsonObject);
  QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);

  for (QWebSocket* socket : clients)
  {
    socket->sendTextMessage(jsonString);
  }

}
