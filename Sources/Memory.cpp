#include <CTRPluginFramework.hpp>
#include "Memory.hpp"
#include "GameTitle.hpp"
#include <utility>
using namespace CTRPluginFramework;

const u32 ARM::UNDEF = 0xDEADCAFE;

const std::vector<std::array<u32, 4>> ARM::_addrs = {
    // POINTERS
    {0x107F58, 0x107F58, 0x107FB4, 0x107FB8},

    // SAVE
    // {0x10531C, 0x10531C, 0x10532C, 0x10532C},

    // {0x20D358, 0x20DAAC, 0x6B0E1C, 0x6B2AC8}, // ANTICHEAT1 ->> UNSURE J3P & J3P_UPD
    // {0x20D51C, 0x20DC70, 0x22916C, 0x22999C}, // ANTICHEAT2
    // {0x230A98, 0x2312F4, 0x660928, 0x662534}, // ANTICHEAT3
    // {0, 0x2311C8, 0, 0x250294}, // ANTICHEAT4
    // {0, 0, 0, 0x2503E0}, // ANTICHEAT5
    // {0, 0, 0, 0x250814}, // ANTICHEAT6
};

const std::vector<std::array<u32, 4>> ARM::_gameHooks = {
    // MONSTERS_CAN_BUY_SHOW
    {0x3DC388, 0x3DCEE0, 0x445F60, 0x44706C},

    // MONSTERS_CAN_BUY_FAMILIES
    {0x33FB60, 0x3406C4, 0X397F78, 0x398FC4},

    // MONSTERS_CAN_BUY_ORIGINAL
    {0x616528, 0x61740C, 0x6B5E64, 0x6B7B34},

    // NOT_ENOUGH_WIFI_COINS
    {0x64C320, 0x64D200, 0x6F3FA8, 0x6F5C2C},

    // UNBUYABLE_MONSTERS (PRO only)
    {ARM::UNDEF, ARM::UNDEF, 0x2E73DC, 0x2E8140}

    // UNLOCKS_DISPLAY
    // {0x61652C, 0x617410, 0x6B5E68, 0x6B7B38}

    // Makes bought monsters always valid: monster data built from the loaded checksum is always seen as valid as long as the cheat is enabled
    // {0, 0x28AA70, 0, 0} // (0xE1A00000)
};

const std::vector<std::array<u32, 4>> ARM::_gameFuncs = {
    // REWARD_WIFI_COINS
    {0x1E1DF8, 0x1E212C, 0x1F4C68, 0x1F53BC},
    
    // GIVE_ITEM
    {0x20D5E8, 0x20DD3C, 0x229220, 0x229A50},

    // UNLOCK TITLE
    {0x1E2944, 0x1E2C78, 0x1F5954, 0x1F60A8}
};

Addr ARM::getAddr(AddrType type) {
    return ARM::_getDataAddr<AddrType>(ARM::_addrs, type);
}

Addr ARM::getGameHook(HookType type) {
    return ARM::_getDataAddr<HookType>(ARM::_gameHooks, type);
}

Addr ARM::getGameFunc(FuncType type) {
    return ARM::_getDataAddr<FuncType>(ARM::_gameFuncs, type);
}

u32 ARM::getBranchInstruction(Addr current, Addr dest, bool link) {
    u32 val = ((dest - current - 8) / 4) & 0xFFFFFF; // Jump offset
    val += link ? 0xB000000 : 0xA000000; // b / bl
    return val + 0xE0000000;
}

u32 Offset::get(const std::array<u32, 2>& offsets) {
    static const int idx = GameTitle::isJ3() ? 0 : 1;
    return offsets[idx];
}

std::array<u32, 2> Offset::STORAGE_DATA = {0x2C0, 0x2F4},
                   Offset::DISC_CLEARED_MONSTER_ID = {0x1CFB0, 0x1D7B4},
                   Offset::DISC_CLEARED_MONSTER_CHECKSUM = {0x1CFD8, 0x1D804},
                   Offset::LIB_MONSTERS = {0x1EBCC, 0x1FB88},
                   Offset::LIB_ABILITIES = {0x1F2CC, 0x2038C},
                   Offset::LIB_SKILLS = {0x1EE4C, 0x1FE20},
                   Offset::LIB_TRAITS = {0x1F28C, 0x20348},
                   Offset::LIB_ITEMS = {0x1ED4C, 0x1FD08},
                   Offset::LIB_TITLES = {0x1FE70, 0x20F38},
                   Offset::INVENTORY = {0x1D208, 0x1E04C},
                   Offset::WIFI_COINS = {0x274, 0x274},
                   Offset::WIFI_STATUES_BITS = {0, 0x1EC},
                   Offset::J3_TRANSFERED_BITS = {0, 0x1CC},
                   
                   Offset::MONSTER_DATA_SIZE = {0xEC, 0xF0},

                   Offset::MSTR_NAME = {4, 4},
                   Offset::MSTR_ID = {0x1C, 0x1C},
                   Offset::MSTR_LVL = {0x2E, 0x2E},
                   Offset::MSTR_MAX_HP = {0x1E, 0x1E},
                   Offset::MSTR_MAX_MP = {0x20, 0x20},
                   Offset::MSTR_CURR_HP = {0x22, 0x22},
                   Offset::MSTR_CURR_MP = {0x24, 0x24},
                   Offset::MSTR_ATK = {0x26, 0x26},
                   Offset::MSTR_DEF = {0x28, 0x28},
                   Offset::MSTR_AGI = {0x2A, 0x2A},
                   Offset::MSTR_WIS = {0x2C, 0x2C},
                   Offset::MSTR_CURR_EXP = {0x30, 0x30},
                   Offset::MSTR_NEXT_EXP = {0x34, 0x34},
                   Offset::MSTR_SKILL = {0x56, 0x56},
                   Offset::MSTR_SKILL_PTS = {0x3A, 0x3A},
                   Offset::MSTR_TRAITS = {0x44, 0x44};

bool PTR::_reach(Addr& outAddr, Addr start, std::vector<u32>& offsets) {
    Addr addr = start;

    for (u32 offset : offsets) {
        addr += offset;

        if (!Process::Read32(addr, addr) || addr == 0)
            return false;
    }
    
    outAddr = addr;
    return true;
}

bool PTR::getSave(Addr& outAddr) {
    static std::vector<u32> offsets = {GameTitle::isJ3P() ? 0x50u : 0x4Cu, 0, 4};
    return PTR::_reach(outAddr, ARM::getAddr(ARM::AddrType::POINTERS), offsets);
}

bool PTR::getInvManager(Addr& outAddr) {
    Addr addr;
    if (!PTR::getSave(addr))
        return false;

    static std::vector<u32> offsets = {GameTitle::isJ3P() ? 0x50u : 0x4Cu, 0, 0x14};
    return PTR::_reach(outAddr, ARM::getAddr(ARM::AddrType::POINTERS), offsets);
}

bool PTR::getTitleManager(Addr& outAddr) {
    static std::vector<u32> offsets = {0x28, 0, 4, 4, GameTitle::isJ3P() ? 0x34u : 0x30u};
    return PTR::_reach(outAddr, ARM::getAddr(ARM::AddrType::POINTERS), offsets);
}