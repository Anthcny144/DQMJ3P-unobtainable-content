#pragma once
#include <CTRPluginFramework.hpp>
#include "GameTitle.hpp"
#include <array>
using namespace CTRPluginFramework;

struct ARM {
    enum AddrType {
        POINTERS
    };

    enum HookType {
        MONSTERS_CAN_BUY_SHOW,
        MONSTERS_CAN_BUY_FAMILIES,
        MONSTERS_CAN_BUY_ORIGINAL,
        NOT_ENOUGH_WIFI_COINS,
        UNBUYABLE_MONSTERS
    };

    enum FuncType {
        REWARD_WIFI_COINS,  // this (useless), coinIdx, amount
        GIVE_ITEM,          // Manager ptr (PTR::getInvManager()), itemID, amount
        GIVE_TITLE          // Manager ptr (PTR::getTitleManager()), titleID
    };

    static Addr getAddr(AddrType type);
    static Addr getGameHook(HookType type);
    static u32 getGameFunc(FuncType type);

    template <typename EnumType>
    static Addr _getDataAddr(const std::vector<std::array<u32, 4>>& addrs, EnumType type) {
        static bool isPro = GameTitle::isJ3P(), isUpdate = GameTitle::isUpdate();
        static int versionIdx = (isPro ? 2 : 0) + (isUpdate ? 1 : 0);
        
        int valueIdx = static_cast<int>(type);
        return addrs[valueIdx][versionIdx];
    }

    static u32 getBranchInstruction(Addr current, Addr dest, bool link = false);

    static const u32 UNDEF;

    template <typename T, class... Args>
    static T call(Addr func, Args... args) {
        return ((T(*)(Args...))func)(args...);
    }

    private:
    static const std::vector<std::array<u32, 4>> _addrs, _gameHooks, _gameFuncs;
};

struct Offset {
    static u32 get(const std::array<u32, 2>& offsets);

    // Global (from save PTR)
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
    WIFI_COINS,
    WIFI_STATUES_BITS,
    J3_TRANSFERED_BITS,

    // Size
    MONSTER_DATA_SIZE,
    
    // Monster data
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

class PTR {
    static bool _reach(Addr& outAddr, Addr start, std::vector<u32>& offsets);

    public:
    static bool getSave(Addr& outAddr);
    static bool getInvManager(Addr& outAddr);
    static bool getTitleManager(Addr& outAddr);
};
