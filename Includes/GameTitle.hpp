#pragma once
#include <CTRPluginFramework.hpp>
using namespace CTRPluginFramework;

enum class VersionBits : u32 {
    J3 = 1 << 0,
    J3_UPD = 1 << 1,
    J3P = 1 << 2,
    J3P_UPD = 1 << 3,

    NONE = 0,
    J3_ALL = J3 | J3_UPD,
    J3P_ALL = J3P | J3P_UPD,
    ALL = J3_ALL | J3P_ALL
};

enum class TitleID : u64 {
    J3 = 0x000400000016AD00,
    J3P = 0x00040000001ACB00,
    UPDATE_MASK = 0x0000000E00000000,
    J3_UPD = TitleID::J3 | TitleID::UPDATE_MASK,
    J3P_UPD = TitleID::J3P | TitleID::UPDATE_MASK
};

struct GameTitle {
    static bool isJ3();
    static bool isJ3P();
    static bool isUpdate();
    static bool isValidVersion();
    static bool init();

    static VersionBits versionBits;

    private:
    static bool _isUpdate;
};