#ifndef BREAKPOINTMANAGER_H
#define BREAKPOINTMANAGER_H

#include <QObject>
#include <QJSValue>
#include <QMap>

class WebSocketServer;
class EmuThread;

class JSBreakPointManager : public QObject
{
    Q_OBJECT
public:
    explicit JSBreakPointManager(QObject *parent = nullptr, EmuThread* emuThread = nullptr, WebSocketServer* in_server = nullptr);
    void loadScript(const QString& inLocation);
    void SetEmuThread(EmuThread* emuThread) {
        emuThread = emuThread;
    }
    QString getLoadedScriptName() const;


public slots:
    void log(const QString& msg) const;
    void registerBreakPoint(const int addr, QJSValue breakPointCallback);
    void continueExecution();
    int readMemotyByte(int addr);
    int readRegister(const int regIndex);
    QString readString(int addr, const QString& encoding);
    void copyToClipboard(const QString& string, int framebuffer_mask = 0);
    void reset();
    void registerUpdateFunction(QJSValue updateCallback);
    QString decodeHex(const QString& inString, const QString& encoding);

private slots:
    void onBreakPoint(int addr);
    void onTimerUpdate();

private:
    EmuThread* emuThread;
    QMap<int, QJSValue> breakPointCallbacks;
    QJSEngine* jsEngine;
    WebSocketServer* server;
    QVector<QJSValue> updateCallbacks;
    class QTimer* timer;
    QString loadedFile;

    QImage copyFrameBuffer(int scren_mask);
};

#endif // BREAKPOINTMANAGER_H
