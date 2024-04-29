#include "DataStorageNDS.h"
#include "memoryviewdialog.h"
#include "ui_memoryviewdialog.h"
#include "HexView/QHexView.h"

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

