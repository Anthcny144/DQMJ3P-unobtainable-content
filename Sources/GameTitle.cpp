#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include "GameTitle.hpp"
#include "CTRPFExtension.hpp"
#include "Macro.hpp"
#include "String.hpp"
#include <unordered_map>
using namespace CTRPluginFramework;

VersionBits GameTitle::versionBits = VersionBits::NONE;
bool GameTitle::_isUpdate = false;

bool GameTitle::init() {
    // check if valid game
    if (!GameTitle::isJ3() && !GameTitle::isJ3P()) {
        MessageBoxPlus::wrap(String::error, String::wrongGame);
        Process::ReturnToHomeMenu();
        return false;
    }

    // determine if update is installed
    u32 count = 0;
    FS_MediaType media = MEDIATYPE_SD;

    // get title count
    if (AM_GetTitleCount(media, &count) == 0) {
        // free space to store titleIds
        u64* titleIDs = (u64*)Macro::FREE_RAM_SPACE_ADDR;

        // get list of installed titles
        u32 actualCount = 0;
        if (AM_GetTitleList(&actualCount, media, count, titleIDs) == 0) {

            // search update title in list
            for (int i = 0; i < actualCount; i++)
                if (titleIDs[i] == static_cast<u64>(GameTitle::isJ3P() ? TitleID::J3P_UPD : TitleID::J3_UPD))
                    _isUpdate = true;
        }
    }

    // check game version (by picking random RAM addresses and reading their value)
    bool validVersion = GameTitle::isValidVersion();

    if (!validVersion) {
        if (!MessageBoxPlus::wrap(String::warning, String::unusableVersion, DialogType::DialogYesNo)) {
            MessageBoxPlus::wrap(String::warning, String::homeMenu);
            Process::ReturnToHomeMenu();
            return false;
        }
    }

    // get version bits
    if (GameTitle::isJ3()) {
        if (GameTitle::isUpdate())
            versionBits = VersionBits::J3_UPD;
        else
            versionBits = VersionBits::J3;
    }

    else if (GameTitle::isJ3P()) {
        if (GameTitle::isUpdate())
            versionBits = VersionBits::J3P_UPD;
        else
            versionBits = VersionBits::J3P;
    }

    return true;
}

bool GameTitle::isJ3() {
    return Process::GetTitleID() == static_cast<u64>(TitleID::J3);
}

bool GameTitle::isJ3P() {
    return Process::GetTitleID() == static_cast<u64>(TitleID::J3P);
}

bool GameTitle::isUpdate() {
    return _isUpdate;
}

bool GameTitle::isValidVersion() {
    // read random ARM values to check if the base game is the same
    std::unordered_map<Addr, u32> checks;
    
    // J3 1.0
    if (!GameTitle::isJ3P() && !GameTitle::isUpdate()) {
        checks = {
            {0x144144, 0xEB06EACD},
            {0x242424, 0x68},
            {0x321144, 0xE3500000},
            {0x550088, 0xE58DA020}
        };
    }

    // J3 1.1
    if (!GameTitle::isJ3P() && GameTitle::isUpdate()) {
        checks = {
            {0x144144, 0xE1A03009},
            {0x242424, 0xE3500000},
            {0x321144, 0x8247FF},
            {0x550088, 0xE59F20C8}
        };
    }

    // J3P 1.0
    if (GameTitle::isJ3P() && !GameTitle::isUpdate()) {
        checks = {
            {0x144144, 0xE3C00003},
            {0x242424, 0xE5957018},
            {0x321144, 0xE5984018},
            {0x550088, 0xEDD70A01}
        };
    }

    // J3P 1.3
    if (GameTitle::isJ3P() && GameTitle::isUpdate()) {
        checks = {
            {0x144144, 0xE3E02000},
            {0x242424, 0xE1510003},
            {0x321144, 0xE3A01002},
            {0x550088, 0xE1A00007}
        };
    }

    u32 read;
    for (const auto& [addr, val] : checks) {
        Process::Read32(addr, read);

        if (read != val)
            return false;
    }

    return true;
}
