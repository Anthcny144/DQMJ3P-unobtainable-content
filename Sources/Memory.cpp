#include <CTRPluginFramework.hpp>
#include "Memory.hpp"
#include "GameTitle.hpp"
#include <utility>
using namespace CTRPluginFramework;

u32 ARM::UNDEF = 0xDEADCAFE;

const std::vector<ARM::ARMData> ARM::data = {
    // SAVE
    ARMData{
        {0x10531C, 0x10531C, 0x10532C, 0x10532C},
        {0x7E59A8, 0x7E69B0, 0x8957F0, 0x897804}
    },

    // HOOK_MONSTERS_CAN_BUY_SHOW
    ARMData{
        {0x3DC388, 0x3DCEE0, 0x445F60, 0x44706C},
        {0xEB6A9328, 0xEB6A9052, 0xEB68EC32, 0xEB68E7EF}
    },

    // HOOK_MONSTERS_CAN_BUY_FAMILIES
    ARMData{
        {0x33FB60, 0x3406C4, 0X397F78, 0x398FC4},
        {0xEB6D0532, 0xEB6D0259, 0xEB6BA42C, 0xEB6BA019}
    },

    // HOOK_MONSTERS_CAN_BUY_ORIGINAL
    ARMData{
        {0x616528, 0x61740C, 0x6B5E64, 0x6B7B34},
        {0xE5900004, 0xE5900004, 0xE5900004, 0xE5900004}
    },

    // HOOK_NOT_ENOUGH_WIFI_COINS
    ARMData{
        {0x64C320, 0x64D200, 0x6F3FA8, 0x6F5C2C},
        {0xE1510002, 0xE1510002, 0xE1510002, 0xE1510002}
    },

    // UNBUYABLE_MONSTERS (PRO only)
    ARMData{
        {ARM::UNDEF, ARM::UNDEF, 0x2E73DC, 0x2E8140},
        {ARM::UNDEF, ARM::UNDEF, 0xE3A00001, 0xE3A00001}
    }

    // ARMAddr{0x20D358, 0x20DAAC, 0x6B0E1C, 0x6B2AC8}, // ANTICHEAT1 ->> UNSURE J3P & J3P_UPD
    // ARMAddr{0x20D51C, 0x20DC70, 0x22916C, 0x22999C}, // ANTICHEAT2
    // ARMAddr{0x230A98, 0x2312F4, 0x660928, 0x662534}, // ANTICHEAT3
    // ARMAddr{0, 0x2311C8, 0, 0x250294}, // ANTICHEAT4
    // ARMAddr{0, 0, 0, 0x2503E0}, // ANTICHEAT5
    // ARMAddr{0, 0, 0, 0x250814}, // ANTICHEAT6
    // ARMAddr{0x61652C, 0x617410, 0x6B5E68, 0x6B7B38}, // HOOK_UNLOCKS_DISPLAY
};

Addr ARM::getAddr(AddrType type) {
    static int idx = ARM::_getDataIdx();
    return ARM::data[static_cast<int>(type)].addr[idx];
}

u32 ARM::getDefaultValue(AddrType type) {
    static int idx = ARM::_getDataIdx();
    return ARM::data[static_cast<int>(type)].value[idx];
}

int ARM::_getDataIdx() {
    bool pro = GameTitle::isJ3P();
    return pro * 2 + (GameTitle::isUpdate() ? 1 : 0);
}

u32 ARM::getBranchInstruction(Addr current, Addr dest, bool link) {
    u32 val = ((dest - current - 8) / 4) & 0xFFFFFF; // jump offset
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

bool PTR::getSave(Addr& outAddr) {
    Addr addr = 0;
    std::vector<u32> offsets = {ARM::getAddr(ARM::AddrType::SAVE), 0, 4};

    for (u32 offset : offsets) {
        addr += offset;

        if (!Process::Read32(addr, addr) || addr == 0)
            return false;
    }
    
    outAddr = addr;
    return true;
}
