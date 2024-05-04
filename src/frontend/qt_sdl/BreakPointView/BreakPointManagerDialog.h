#ifndef BREAKPOINTMANAGERDIALOG_H
#define BREAKPOINTMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class BreakPointManagerDialog;
}

class BreakPointManagerDialog;
class EmuThread;
class QStringListModel;
class QJSEngine;

class BreakPointManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BreakPointManagerDialog(QWidget *parent = nullptr, EmuThread* emuThread = nullptr);
    ~BreakPointManagerDialog();

    static BreakPointManagerDialog* currentDlg;
    static BreakPointManagerDialog* openDlg(QWidget* parent, EmuThread* emuThread)
    {
        if (currentDlg)
        {
            currentDlg->activateWindow();
            return currentDlg;
        }

        currentDlg = new BreakPointManagerDialog(parent, emuThread);
        currentDlg->show();
        return currentDlg;
    }
    static void closeDlg()
    {
        currentDlg = nullptr;
    }

private slots:
    void on_pushButton_clicked();

private slots:
    void on_removeSelectedButton_clicked();

private slots:
    void on_addBreakButton_clicked();

private:
    Ui::BreakPointManagerDialog *ui;
    EmuThread* emuThread;
    QStringListModel* breakPointListModel = nullptr;
    QJSEngine* jsEngine;
    class JSBreakPointManager* jsBreakPointManager;
};

#endif // BREAKPOINTMANAGERDIALOG_H
