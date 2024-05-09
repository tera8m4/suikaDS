#include "DataStorageNDS.h"
#include "memoryviewdialog.h"
#include "qdebug.h"
#include "ui_memoryviewdialog.h"
#include "HexView/QHexView.h"

#include <QFileDialog>
#include <QSaveFile>

MemoryViewDialog* MemoryViewDialog::currentDlg = nullptr;

MemoryViewDialog::MemoryViewDialog(QWidget *parent, EmuThread* emuThread) :
    QDialog(parent),
    ui(new Ui::MemoryViewDialog),
    emuThread(emuThread)
{
    ui->setupUi(this);

    hexView = new QHexView(ui->hexViewContainer);
    hexView->setData(new DataStorageNDS(emuThread));
}

MemoryViewDialog::~MemoryViewDialog()
{
    delete ui;
}

void MemoryViewDialog::on_goToAddressButton_clicked()
{
    const QString& offsetText = ui->offset->text();
    bool ok;
    const int offsetAddr = offsetText.toInt(&ok, 16);
    if (ok) {
        hexView->showFromOffset(offsetAddr);
    }
    ui->offset->clear();
}

void MemoryViewDialog::done(int r)
{
    QDialog::done(r);
    closeDlg();
}


void MemoryViewDialog::on_dumpMemoryButton_clicked()
{
    const QString fname = QFileDialog::getSaveFileName(nullptr, "test save name", ".", "dump (*.bin)" );
    qDebug() << "name is : " << fname;
//    QFile file(fname);
    DataStorageNDS storage(emuThread);

    QByteArray a = storage.getData(0, storage.size());
    QSaveFile file(fname);
    file.open(QIODevice::WriteOnly);
    file.write(a)   ;
    // Calling commit() is mandatory, otherwise nothing will be written.
    file.commit();
}

