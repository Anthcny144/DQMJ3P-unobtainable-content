#pragma once
#include <CTRPluginFramework.hpp>
#include <array>
using namespace CTRPluginFramework;

struct ARM {
    enum AddrType {
        SAVE = 0,
        HOOK_MONSTERS_CAN_BUY_SHOW,
        HOOK_MONSTERS_CAN_BUY_FAMILIES,
        HOOK_MONSTERS_CAN_BUY_ORIGINAL,
        HOOK_NOT_ENOUGH_WIFI_COINS,
        UNBUYABLE_MONSTERS
    };

    static Addr getAddr(AddrType type);
    static u32 getDefaultValue(AddrType type);
    static u32 getBranchInstruction(Addr current, Addr dest, bool link = false);
    static u32 UNDEF;

    private:
    struct ARMData {
        std::array<Addr, 4> addr;
        std::array<u32, 4> value;
    };

    static const std::vector<ARMData> data;
    static int _getDataIdx();
};

struct Offset {
    static u32 get(const std::array<u32, 2>& offsets);

    // global (from save PTR)
    static std::array<u32, 2>
    STORAGE_DATA,
    DISC_CLEARED_MONSTER_ID,
    DISC_CLEARED_MONSTER_CHECKSUM,
    LIB_MONSTERS,
    LIB_ABILITIES,
    LIB_SKILLS,
    LIB_TRAITS,
    LIB_ITEMS,
    LIB_TITLES,
    INVENTORY,
    WIFI_STATUES_BITS,
    J3_TRANSFERED_BITS,

    // size
    MONSTER_DATA_SIZE,
    
    // monster data
    MSTR_NAME,
    MSTR_ID,
    MSTR_LVL,
    MSTR_MAX_HP,
    MSTR_MAX_MP,
    MSTR_CURR_HP,
    MSTR_CURR_MP,
    MSTR_ATK,
    MSTR_DEF,
    MSTR_AGI,
    MSTR_WIS,
    MSTR_CURR_EXP,
    MSTR_NEXT_EXP,
    MSTR_SKILL,
    MSTR_SKILL_PTS,
    MSTR_TRAITS;
};

struct PTR {
    static bool getSave(Addr& outAddr);
};
