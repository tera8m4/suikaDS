#include "BreakPointManagerDialog.h"
#include "ui_BreakPointManagerDialog.h"
#include "EmuThread.h"
#include "BreakPointManager/JSBreakPointManager.h"
#include <QtQml/QJSEngine>
#include <QFile>

#include <QFileDialog>
#include <QStringListModel>

BreakPointManagerDialog* BreakPointManagerDialog::currentDlg = nullptr;

BreakPointManagerDialog::BreakPointManagerDialog(QWidget *parent, EmuThread* emuThread, JSBreakPointManager* breakPointManager) :
    QDialog(parent),
    ui(new Ui::BreakPointManagerDialog),
    emuThread(emuThread)
{
    ui->setupUi(this);

    breakPointListModel = new QStringListModel(this);
    ui->listView->setModel(breakPointListModel);

    jsBreakPointManager = breakPointManager;
}

BreakPointManagerDialog::~BreakPointManagerDialog()
{
    delete ui;
}

void BreakPointManagerDialog::done(int r)
{
    QDialog::done(r);
    closeDlg();
}

void BreakPointManagerDialog::on_pushButton_clicked()
{
    const QString fname = QFileDialog::getOpenFileName(this, "Load game script");
    jsBreakPointManager->loadScript(fname);
}


void BreakPointManagerDialog::on_ContinueButton_clicked()
{
    jsBreakPointManager->continueExecution();
}

