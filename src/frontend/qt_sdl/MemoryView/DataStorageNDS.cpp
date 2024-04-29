#include "DataStorageNDS.h"
#include "MemConstants.h"
#include "RAMInfoDialog.h"
#include "types.h"
#include "EmuThread.h"

using namespace melonDS;

DataStorageNDS::DataStorageNDS(EmuThread* emuThread) : emuThread{emuThread}
{

}

QByteArray DataStorageNDS::getData(std::size_t position, std::size_t length)
{
    emuThread->emuPause();

    QByteArray arr;

    const u32 start = 0x02000000 + position;
    const u32 end = 0x02000000 + position + length;
    for (u32 addr = start; addr < end; addr += 1)
    {
        const s32& value = GetMainRAMValue(*emuThread->NDS, addr, ramInfo_OneByte);
        arr.push_back(value);
    }

    emuThread->emuUnpause();

    return arr;
}

std::size_t DataStorageNDS::size() {
    return melonDS::MainRAMMaxSize;
}
