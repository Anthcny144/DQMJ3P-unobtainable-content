#pragma once
#include <CTRPluginFramework.hpp>
#include <cmath>
using namespace CTRPluginFramework;

struct String {
    static const std::string

    // yes no on off
    yes,
    no,
    on,
    off,

    // game names
    dqmj3p,
    dqmj3,

    // main 3gx
    unobtainableContent,
    discordLink,
    pluginNotes,
    welcome,
    pressSelect,
    homeMenu,
    cantWriteFreeMem,
    pressBExit,
    cheat_unobtainableContent,
    notes_unobtainableContent,

    // global messagebox
    success,
    info,
    question,
    warning,
    error,

    // invalid version messagebox
    wrongGame,
    unusableVersion,

    // cheats
    all,
    unlockedSkills,
    unlockedItems,
    unlockedTitles,
    monstersCheatON,
    monstersCheatONEmuWarning,
    monstersCheatOFF,
    howToUnlockMonsters,
    sirloinConfirm,
    sirloinSuccess,
    sirloinWait,
    sirloinError,
    sirloinNotUnlockedAll,
    unlockOnceAlreadyObtained1,
    unlockOnceAlreadyObtained2,
    itemUnlocked,

    // keyboard
    chooseOption,
    chooseCategory,
    categoryMonsters,
    categorySkills,
    categoryItems,
    categoryTitles,
    chooseMonster,
    chooseSkillUnlock,
    chooseItemUnlock,
    chooseTitleUnlock,
    unlockMethods,

    // unlock methods
    downloadableDisk,
    starterTrade,
    j3Transfer,
    streetPass,
    spotPass,
    spotPassDisk,
    spotpassBook,
    streetPass50Battles,
    wildcardLeonyx,
    unlockKingSP,
    unlockDemonkingSP,
    eastStationJ3,
    westStationJ3,
    eastStationJ3P,
    westStationJ3P,
    unlockSirloin,
    transferTicket,
    terryStatue,
    mireilleStatue,
    iruStatue,
    lucaStatue,
    wifiBattle1,
    wifiBattle10,
    wifiBattle100,
    wifiPts10,
    wifiPts500,
    wifiPts1000,
    wifiPts4000,
    wifiPts5000,
    wifiPts9999,
    meetStreetpass10,
    meetStreetpass100,
    meetStreetpass1000,
    winStreetpass10,
    winStreetpass100,
    winStreetpass1000,
    mgpPlay,
    mgp10000,
    mgp1000,
    mgp100,
    mgp8,
    mgp1,
    cgpPlay,
    cgp10000,
    cgp1000,
    cgp100,
    cgp8,
    cgp1,
    commu1,
    commu2,
    commu3,

    empty;

    template<typename T>
    static inline std::string plural(T value) {
        return (value == 0 || value == 1) ? "" : "s";
    }

    static inline std::string gradient(const std::string& text, Color from, Color to, float power = 0.5f) {
        std::vector<Color> cols;
        
        for (int i = 0; i < text.length(); i++) {
            float ratio = (text.length() == 1) ? 0.0f : float(i) / (text.length() - 1),
                  weightRatio = pow(ratio, std::log(0.5f) / std::log(power));

            u8 r = from.r + (to.r - from.r) * weightRatio,
               g = from.g + (to.g - from.g) * weightRatio,
               b = from.b + (to.b - from.b) * weightRatio;

            cols.emplace_back(Color(r, g, b, 0xFF));
        }

        std::string newText = "";
        for (int i = 0; i < text.length(); i++) {
            newText += cols[i];
            newText += text.substr(i, 1);
        }

        return newText;
    }

    static inline std::string bold(const std::string& text) {
        return ToggleDrawMode(Render::BOLD) << text << ToggleDrawMode(Render::BOLD);
    }

    static inline int centerSysFont(const std::string& text, int startX, int width) {
        const int fontWidth = OSD::GetTextWidth(true, text);
        return (int)(width / 2 - fontWidth / 2 + startX);
    }
};