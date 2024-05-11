#include "BreakPointManagerDialog.h"
#include "ui_BreakPointManagerDialog.h"
#include "EmuThread.h"
#include "BreakPointManager/JSBreakPointManager.h"
#include <QtQml/QJSEngine>
#include <QFile>

#include <QFileDialog>
#include <QStringListModel>
#include <qdebug.h>

static const QString SCRIPTS_FOLDER("text-hook-scripts");

FTextHookScriptInfo FTextHookScriptInfo::Create(const QString& inFilePath, QFile &inFile) {
    FTextHookScriptInfo info;
    info.FilePath = inFilePath;

    QString tmpLine = inFile.readLine();
    const QString beginMetaData = "/*";
    if (tmpLine.contains(beginMetaData)) {
        while (!tmpLine.contains("*/") && !inFile.atEnd()) {
            tmpLine = inFile.readLine();
            const auto& parts = tmpLine.split(":");
            if (parts[0].contains("rom-code", Qt::CaseSensitivity::CaseInsensitive)) {
                info.RomCode = parts[1].trimmed();
            } else if (parts[0].contains("name", Qt::CaseSensitivity::CaseInsensitive)) {
                info.DisplayName = parts[1].trimmed();
            }
        }
    }

    if (info.DisplayName.isEmpty()) {
        info.DisplayName = inFile.fileName().split(".")[0];
    }

    return info;
}

BreakPointManagerDialog* BreakPointManagerDialog::currentDlg = nullptr;

BreakPointManagerDialog::BreakPointManagerDialog(QWidget *parent, EmuThread* emuThread, JSBreakPointManager* breakPointManager) :
    QDialog(parent),
    ui(new Ui::BreakPointManagerDialog),
    emuThread(emuThread)
{
    ui->setupUi(this);

    breakPointListModel = new QStringListModel(this);


    jsBreakPointManager = breakPointManager;

    QDir scripts(SCRIPTS_FOLDER);
    for (const auto& fileInfo : scripts.entryInfoList(QDir::Files)) {
        const QString& filePath = fileInfo.absoluteFilePath();

        QFile file(filePath);
        file.open(QFile::ReadOnly);
        scriptInfos.append(FTextHookScriptInfo::Create(filePath, file));
    }

    for (const auto& info : scriptInfos)
    {
        if(breakPointListModel->insertRow(breakPointListModel->rowCount())) {
            QModelIndex index = breakPointListModel->index(breakPointListModel->rowCount() - 1, 0);
            breakPointListModel->setData(index, info.DisplayName);
        }
        qDebug() << "text hook script:" << info.DisplayName;
    }

    ui->listView->setModel(breakPointListModel);

    updateLoadedFileWidget();

}

void BreakPointManagerDialog::updateLoadedFileWidget() {
    const QString& loadedFile = jsBreakPointManager->getLoadedScriptName();
    if (loadedFile.isEmpty()) {
        ui->loadedScript->setText("Double click on a game name or load a custom file");
    } else {
        ui->loadedScript->setText("Loaded file: " + loadedFile);
    }
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

    updateLoadedFileWidget();
}


void BreakPointManagerDialog::on_ContinueButton_clicked()
{
    jsBreakPointManager->continueExecution();
}


void BreakPointManagerDialog::on_listView_doubleClicked(const QModelIndex &index)
{
    const int row = index.row();
    if (row < scriptInfos.size()) {
        jsBreakPointManager->loadScript(scriptInfos[row].FilePath);
    }

    updateLoadedFileWidget();
}

