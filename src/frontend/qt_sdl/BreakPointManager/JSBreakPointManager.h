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

public slots:
    void log(const QString& msg) const;
    void registerBreakPoint(const int addr, QJSValue breakPointCallback);
    void onBreakPoint(int addr);
    void continueExecution();
    uint32_t readMemotyByte(int addr);
    uint32_t readRegister(const int regIndex);
    QString readString(uint32_t addr, QString encoding);
    void copyToClipboard(const QString& string);

private:
    EmuThread* emuThread;
    QMap<int, QJSValue> breakPointCallbacks;

};

#endif // BREAKPOINTMANAGER_H
