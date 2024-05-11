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
class JSBreakPointManager;

class BreakPointManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BreakPointManagerDialog(QWidget *parent = nullptr, EmuThread* emuThread = nullptr, JSBreakPointManager* breakPointManager = nullptr);
    ~BreakPointManagerDialog();

    static BreakPointManagerDialog* currentDlg;
    static BreakPointManagerDialog* openDlg(QWidget* parent, EmuThread* emuThread, JSBreakPointManager* breakPointManager)
    {
        if (currentDlg)
        {
            currentDlg->activateWindow();
            return currentDlg;
        }

        currentDlg = new BreakPointManagerDialog(parent, emuThread, breakPointManager);
        currentDlg->show();
        return currentDlg;
    }
    static void closeDlg()
    {
        currentDlg = nullptr;
    }

private slots:
    void on_ContinueButton_clicked();

private slots:
    void on_pushButton_clicked();
    void done(int r);

private:
    Ui::BreakPointManagerDialog *ui;
    EmuThread* emuThread;
    QStringListModel* breakPointListModel = nullptr;
    QJSEngine* jsEngine;
    class JSBreakPointManager* jsBreakPointManager;
};

#endif // BREAKPOINTMANAGERDIALOG_H
