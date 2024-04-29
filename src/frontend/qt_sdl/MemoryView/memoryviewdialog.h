#ifndef MEMORYVIEWDIALOG_H
#define MEMORYVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class MemoryViewDialog;
}

class EmuThread;
class MemoryViewDialog;
class QHexView;

class MemoryViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MemoryViewDialog(QWidget *parent = nullptr, EmuThread* emuThread = nullptr);
    ~MemoryViewDialog();


    static MemoryViewDialog* currentDlg;
    static MemoryViewDialog* openDlg(QWidget* parent, EmuThread* emuThread)
    {
        if (currentDlg)
        {
            currentDlg->activateWindow();
            return currentDlg;
        }

        currentDlg = new MemoryViewDialog(parent, emuThread);
        currentDlg->show();
        return currentDlg;
    }
    static void closeDlg()
    {
        currentDlg = nullptr;
    }

private slots:
    void on_goToAddressButton_clicked();

private:
    Ui::MemoryViewDialog *ui;
    EmuThread* emuThread;
    QHexView* hexView = nullptr;
};

#endif // MEMORYVIEWDIALOG_H
