#ifndef BREAKPOINTMANAGER_H
#define BREAKPOINTMANAGER_H

#include <QObject>
#include <QJSValue>
#include <QMap>

class EmuThread;

class JSBreakPointManager : public QObject
{
    Q_OBJECT
public:
    explicit JSBreakPointManager(QObject *parent = nullptr, EmuThread* emuThread = nullptr);
    void loadScript(const QString& inLocation);
    void SetEmuThread(EmuThread* emuThread) {
        emuThread = emuThread;
    }


public slots:
    void log(const QString& msg) const;
    void registerBreakPoint(const int addr, QJSValue breakPointCallback);
    void continueExecution();
    uint32_t readMemotyByte(int addr);
    uint32_t readRegister(const int regIndex);
    QString readString(uint32_t addr, QString encoding);
    void copyToClipboard(const QString& string);
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
    QVector<QJSValue> updateCallbacks;
    class QTimer* timer;

};

#endif // BREAKPOINTMANAGER_H
