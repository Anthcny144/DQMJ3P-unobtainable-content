#include <CTRPluginFramework.hpp>
#include "Game.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
#include <vector>
using namespace CTRPluginFramework;

bool Game::isLoaded() {
    u32 val;
    Process::Read32(ARM::getAddr(ARM::AddrType::SAVE), val);
    Process::Read32(val, val);

    return val != 0;
}

bool Game::unlock(u32 offset, u16 id) {
    Addr addr;
    if (!PTR::getSave(addr))
        return false;

    if (Game::isUnlocked(offset, id))
        return false;

    u32 byteOffs = id / 8;
    u32 bitOffs = id % 8;
    addr += (offset + byteOffs);

    u8 byte;
    Process::Read8(addr, byte);

    byte |= (1 << bitOffs);
    Process::Write8(addr, byte);
    return true;
}

bool Game::isUnlocked(u32 offset, u16 id) {
    Addr addr;
    if (!PTR::getSave(addr))
        return false;

    u32 byteOffs = id / 8;
    u32 bitOffs = id % 8;
    addr += (offset + byteOffs);
    
    u8 byte;
    Process::Read8(addr, byte);

    return (byte & (1 << bitOffs)) != 0;
}

void Game::increaseItemCount(u16 itemId) {
    Addr addr;
    if (!PTR::getSave(addr))
        return;

    u16 count;
    addr += Offset::get(Offset::INVENTORY);
    addr += itemId * sizeof(itemId);

    Process::Read16(addr, count);
    count = std::min(static_cast<u16>(count + 1), static_cast<u16>(Macro::MAX_ITEM_COUNT));
    
    Process::Write16(addr, count);
}