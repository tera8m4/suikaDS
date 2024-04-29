#ifndef DATASTORAGENDS_H
#define DATASTORAGENDS_H

#include "HexView/QHexView.h"

class EmuThread;

class DataStorageNDS : public QHexView::DataStorage
{
public:
    DataStorageNDS(EmuThread* emuThread);
    virtual QByteArray getData(std::size_t position, std::size_t length) override;
    virtual std::size_t size() override;

private:
    EmuThread* emuThread;
};

#endif // DATASTORAGENDS_H
