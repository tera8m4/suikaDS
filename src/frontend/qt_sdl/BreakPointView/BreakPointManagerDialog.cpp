#include "BreakPointManagerDialog.h"
#include "ui_BreakPointManagerDialog.h"
#include "EmuThread.h"
#include "BreakPointManager/JSBreakPointManager.h"
#include <QtQml/QJSEngine>
#include <QFile>

#include <QStringListModel>

BreakPointManagerDialog* BreakPointManagerDialog::currentDlg = nullptr;

BreakPointManagerDialog::BreakPointManagerDialog(QWidget *parent, EmuThread* emuThread) :
    QDialog(parent),
    ui(new Ui::BreakPointManagerDialog),
    emuThread(emuThread)
{
    ui->setupUi(this);

    breakPointListModel = new QStringListModel(this);
    ui->listView->setModel(breakPointListModel);

    jsBreakPointManager = new JSBreakPointManager(this, emuThread);

    jsEngine = new QJSEngine(this);
    jsEngine->globalObject().setProperty("bpManager", jsEngine->newQObject(jsBreakPointManager));
}

BreakPointManagerDialog::~BreakPointManagerDialog()
{
    delete ui;
}

void BreakPointManagerDialog::on_addBreakButton_clicked()
{
    const QString& addrString = ui->breakPointAddr->text();

    bool ok = false;
    const int breakPointAddr = addrString.toInt(&ok, 16);
    if (ok)
    {
        breakPointListModel->insertRows(breakPointListModel->rowCount(), 1);
        breakPointListModel->setData(breakPointListModel->index(breakPointListModel->rowCount() - 1), addrString);
        emuThread->addBreakPoint(breakPointAddr);

        ui->breakPointAddr->clear();
    }
}


void BreakPointManagerDialog::on_removeSelectedButton_clicked()
{
    const QModelIndexList& selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
    for (const auto& index : selectedIndexes) {
        bool ok = false;
        int addr = index.data().toString().toInt(&ok, 16);
        if (ok) {
            emuThread->removeBreakPoint(addr);
        }
        breakPointListModel->removeRows(index.row(), 1);
    }
}


void BreakPointManagerDialog::on_pushButton_clicked()
{
    if (jsEngine)
    {
        QString fileName = "/home/tera8/tmp/1.js";
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        jsEngine->evaluate(file.readAll(), fileName);
    }
}

