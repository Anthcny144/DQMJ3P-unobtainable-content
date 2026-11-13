#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "Content.hpp"
#include "GameTitle.hpp"
#include "Memory.hpp"
#include "Text.hpp"
using namespace CTRPluginFramework;

Content::Content(const std::string& name, const u16 id, Gradient gradient, const std::string& unlockMethod, VersionBits version)
: name(name), id(id), unlockMethod(unlockMethod), version(version)
{
    // Single color
    if (gradient.power == -1)
        colorName = gradient.first << name;

    // Gradient
    else
        colorName = Text::gradient(name, gradient.first, gradient.second, gradient.power);
}

Monster::Monster(const std::string& name,
                 u32 iconIdx,
                 Gradient gradient,
                 u16 id,
                 const std::string& unlockMethod,
                 u32 checksum,
                 VersionBits version) :
Content(name, id, gradient, unlockMethod, version),
iconIdx(iconIdx), checksum(checksum) {}

CustomIcon Monster::getIcon(std::vector<CustomIcon::Pixel>& iconsPixels) {
    return CustomIconPlus::loadIcon(iconsPixels, this->iconIdx);
}

std::vector<Monster>& Monster::getList() {
    static std::vector<Monster> list = {
        Monster(Text::get("MONSTER_01"), 0, Gradient(Color::Red, Color::Blue), 0x214, Text::get("METHOD_DISK"), 0xD1279F38, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_02"), 1, Gradient(Color(0xFF3AE4FF), Color::White), 0x332, Text::get("METHOD_DISK"), 0xC0894415, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_03"), 2, Gradient(Color(0x0087FFFF), Color(0x4F5757FF)), 0x36B, Text::get("METHOD_STARTER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_04"), 3, Gradient(Color(0xF24BBDFF), Color(0x4DC5FEFF), 0.75f), 0x348, Text::get("METHOD_DISK"), 0xE5C562D3, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_05"), 4, Gradient(Color(0x0754C0FF), Color(0xE2A400FF), 0.85f), 0x311, Text::get("METHOD_DISK"), 0xDD87CA2B, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_06"), 5, Gradient(Color(0x4F5757FF), Color(0xFFB200FF), 0.85f), 0x312, Text::get("METHOD_DISK"), 0x13BA6929, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_07"), 6, Gradient(Color(0x0087FFFF), Color::Yellow, 0.95f), 0x1DE, Text::get("METHOD_SPOTPASS"), 0, VersionBits::ALL),
        Monster(Text::get("MONSTER_08"), 7, Gradient(Color(0xF0CB00FF), Color(0xFCFA0BFF)), 0x389, Text::get("METHOD_DISK"), 0x2A168841, VersionBits::J3P_UPD),

        Monster(Text::get("MONSTER_09"), 8, Gradient(Color(0x5AA6AAFF), Color(0x39175BFF)), 0x2CC, Text::get("METHOD_DISK"), 0x86D6931, VersionBits::J3P_ALL),

        Monster(Text::get("MONSTER_10"), 9, Gradient(Color(0xB9BEB9FF), Color(0xB04E1CFF)), 0x371, Text::get("METHOD_DISK"), 0x69ABD8B1, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_11"), 10, Gradient(Color(0xCFA66DFF)), 0x368, Text::get("METHOD_STARTER"), 0, VersionBits::J3P_ALL),
        
        Monster(Text::get("MONSTER_12"), 11, Gradient(Color(0x7000BDFF), Color(0xFBA500FF)), 0x1E1, Text::get("METHOD_DISK"), 0x3DC73591, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_13"), 12, Gradient(Color(0xFFD5EDFF), Color(0xF3CDB1FF), 0.75f), 0x309, Text::get("METHOD_STREETPASS"), 0x64764325, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_14"), 13, Gradient(Color(0xFBEB76FF)), 0x361, Text::get("METHOD_DISK"), 0xC51B2BCB, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_15"), 14, Gradient(Color(0xFBEB76FF)), 0x362, Text::get("METHOD_DISK"), 0xEDBB68C3, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_16"), 15, Gradient(Color(0xFBEB76FF)), 0x363, Text::get("METHOD_DISK"), 0xA6764EF, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_17"), 16, Gradient(Color(0xFBEB76FF)), 0x364, Text::get("METHOD_DISK"), 0xF6FAA1E6, VersionBits::J3P_ALL),

        Monster(Text::get("MONSTER_18"), 17, Gradient(Color(Color::Red), Color(0x575757FF), 0.70f), 0x21E, Text::get("METHOD_DISK"), 0xEE70D7E9, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_19"), 18, Gradient(Color(0x274DE8FF)), 0x339, Text::get("METHOD_STREETPASS"), 0xCC7A12BA, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_20"), 19, Gradient(Color(0xDFD410FF), Color(0x545353FF)), 0x35E, Text::get("METHOD_DISK"), 0x3D606EDA, VersionBits::J3P_ALL),

        Monster(Text::get("MONSTER_21"), 20, Gradient(Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), 0x1C7, Text::get("METHOD_DISK"), 0x4FD5E3AC, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_22"), 21, Gradient(Color(0x2B4047FF), Color::Red, 0.90f), 0x1C6, Text::get("METHOD_DISK"), 0x7E36B391, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_23"), 22, Gradient(Color(0xEEEEEEFF), Color(0x50D3E0FF), 0.90f), 0x1C5, Text::get("METHOD_WILDCARD_LEONYX", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_WILDCARD"), Color(0xEEEEEEFF), Color(0x50D3E0FF), 0.90f)) << Color::Silver, Text::bold(Text::gradient(Text::get("HELPER_MONSTER_LEONYX"), Color(0xFEC610FF), Color(0xC2976DFF))) << Color::Silver}), 0, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_24"), 23, Gradient(Color(0x342C6DFF), Color(0xD87A1BFF)), 0x36A, Text::get("METHOD_STARTER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_25"), 24, Gradient(Color(0xB95F03FF), Color(0xE49E14FF)), 0x369, Text::get("METHOD_STARTER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_26"), 25, Gradient(Color(0xFCE411FF), Color::White, 0.80f), 0x2E5, Text::get("METHOD_DISK"), 0x81C4CBF4, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_27"), 26, Gradient(Color(0xF4AACBFF), Color::White, 0.80f), 0x34B, Text::get("METHOD_DISK"), 0x2D787476, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_28"), 27, Gradient(Color(0xF7BE8BFF), Color::White), 0x2CF, Text::get("METHOD_DISK"), 0xBBE9EE08, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_29"), 28, Gradient(Color(0xFEC610FF), Color(0xC2976DFF)), 0x1D4, Text::get("METHOD_WILDCARD_LEONYX", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_WILDCARD"), Color(0xEEEEEEFF), Color(0x50D3E0FF), 0.90f)) << Color::Silver, Text::bold(Text::gradient(Text::get("HELPER_MONSTER_LEONYX"), Color(0xFEC610FF), Color(0xC2976DFF)))}), 0, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_30"), 29, Gradient(Color(0xFEC610FF), Color(0xC2976DFF)), 0x342, Text::get("METHOD_DISK"), 0x1D409934, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_31"), 30, Gradient(Color(0xFCAE00FF), Color(0xA58D00FF), 0.80f), 0x228, Text::get("METHOD_DISK"), 0x13D3A4C6, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_32"), 31, Gradient(Color(0xF0F0F0FF), Color(0x858585FF), 0.80f), 0x229, Text::get("METHOD_DISK"), 0xC72554A, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_33"), 32, Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E0, Text::get("METHOD_J3_TRANSFER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_34"), 33, Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E3, Text::get("METHOD_J3_TRANSFER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_35"), 34, Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E6, Text::get("METHOD_J3_TRANSFER"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_36"), 35, Gradient(Color(0xC81519FF), Color(0xD5BE75FF), 0.40f), 0x1E7, Text::get("METHOD_SPOTPASS_DISK", {Text::getUniversal("DQMJ3") << Color::Silver, Text::getUniversal("DQMJ3P") << Color::Silver, Text::get("METHOD_DISK")}), 0xCEEA5CC2, VersionBits::ALL),
        Monster(Text::get("MONSTER_37"), 36, Gradient(Color::Red, Color(0xFFEB0FFF), 0.30f), 0x1E2, Text::get("METHOD_STREETPASS_50"), 0, VersionBits::J3_ALL),
        Monster(Text::get("MONSTER_38"), 37, Gradient(Color(0x32EEC3FF), Color(0xE30098FF)), 0x34E, Text::get("METHOD_STREETPASS_50"), 0, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_39"), 38, Gradient(Color::Red, Color(0x404040FF)), 0x35F, Text::get("METHOD_DISK"), 0xBF06F924, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_40"), 39, Gradient(Color(0x72574CFF), Color(0x404040FF)), 0x360, Text::get("METHOD_DISK"), 0xFCCD4A20, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_41"), 40, Gradient(Color(0x608795FF), Color(0x382267FF)), 0x326, Text::get("METHOD_DISK"), 0x4B54D667, VersionBits::J3P_ALL),
        Monster(Text::get("MONSTER_42"), 41, Gradient(Color(0x4AC227FF), Color(0x8C39AAFF), 0.40f), 0x333, Text::get("METHOD_DISK"), 0xB52DD054, VersionBits::J3P_ALL)
    };

    return list;
};

Skill::Skill(const std::string& name,
             Gradient gradient,
             u16 id,
             u16 bookId,
             const std::string& bookName,
             const std::string& unlockMethod,
             VersionBits version) :
Content(name, id, gradient, unlockMethod, version),
bookId(bookId), bookName(bookName) {}

std::vector<Skill>& Skill::getList() {
    static std::vector<Skill> list = {
        // https://wikiwiki.jp/dqdic3rd/%E3%80%90%E3%82%B9%E3%83%A9%E3%82%A4%E3%83%80%E3%83%BC%E3%82%AC%E3%83%BC%E3%83%AB%E3%80%91q
        Skill(Text::get("SKILL_01"), Gradient(Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), 0x1F9, 0x375, Text::gradient(Text::get("SKILL_01_BOOK"), Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), Text::get("METHOD_KING_SP", {Text::getUniversal("DQMJ3P") << Color::Silver, Text::bold(Text::gradient(Text::get("HELPER_MONSTER_RUGIUS"), Color::Red, Color(0x404040FF))) << Color::Silver, Text::getUniversal("DQMJ3") << Color::Silver, Text::bold(Text::gradient(Text::get("HELPER_MONSTER_WULFSPADE_ACE"), Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f)) << Color::Silver}), VersionBits::ALL),
        Skill(Text::get("SKILL_02"), Gradient(Color(0x2B4047FF), Color::Red, 0.90f), 0x1FA, 0x376, Text::gradient(Text::get("SKILL_02_BOOK"), Color(0x2B4047FF), Color::Red, 0.90f), Text::get("METHOD_DEMONKING_SP", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_ACE_OF_SPADES"), Color(0x2B4047FF), Color::Red, 0.90f)) << Color::Silver}), VersionBits::J3_ALL),
        Skill(Text::get("SKILL_03"), Gradient(Color::Red, Color::White), 0x1FB, 0x377, Text::gradient(Text::get("SKILL_03_BOOK"), Color::Red, Color::White), Text::get("METHOD_SPOTPASS_BOOK"), VersionBits::J3_ALL),
        Skill(Text::get("SKILL_04"), Gradient(Color::Orange, Color::Lime, 0.80f), 0x1ED, 0x369, Text::gradient(Text::get("SKILL_04_BOOK"), Color::Orange, Color::Lime, 0.80f), Text::get("METHOD_EAST_STATION_J3", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_KING_HEALSLIME"), Color(0x0087FFFF), Color::Yellow, 0.95f)) << Color::Silver}), VersionBits::ALL),
        Skill(Text::get("SKILL_05"), Gradient(Color::DodgerBlue, Color::Orange, 0.20f), 0x1EE, 0x36A, Text::gradient(Text::get("SKILL_05_BOOK"), Color::DodgerBlue, Color::Orange, 0.20f), Text::get("METHOD_WEST_STATION_J3", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_KING_HEALSLIME"), Color(0x0087FFFF), Color::Yellow, 0.95f)) << Color::Silver}), VersionBits::ALL),
        Skill(Text::get("SKILL_06"), Gradient(ColorPlus::Pink, Color::Lime, 0.80f), 0x241, 0x435, Text::gradient(Text::get("SKILL_06_BOOK"), ColorPlus::Pink, Color::Lime, 0.80f), Text::get("METHOD_EAST_STATION_J3P", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_SLIDER_GIRL"), Color(0xF24BBDFF), Color(0x4DC5FEFF), 0.75f)) << Color::Silver}), VersionBits::J3P_ALL),
        Skill(Text::get("SKILL_07"), Gradient(Color::DodgerBlue, ColorPlus::Pink, 0.20f), 0x242, 0x436, Text::gradient(Text::get("SKILL_07_BOOK"), Color::DodgerBlue, ColorPlus::Pink, 0.20f), Text::get("METHOD_WEST_STATION_J3P", {Text::bold(Text::gradient(Text::get("HELPER_MONSTER_SLIDER_GIRL"), Color(0xF24BBDFF), Color(0x4DC5FEFF), 0.75f)) << Color::Silver}), VersionBits::J3P_ALL)
    };
    
    return list;
};

Item::Item(const std::string& name,
           Gradient gradient,
           u16 id,
           bool unlockableOnce,
           const std::string& unlockMethod,
           std::function<void(Item*)> onUnlock,
           VersionBits version) :
Content(name, id, gradient, unlockMethod, version),
unlockableOnce(unlockableOnce), onUnlock(onUnlock) {}

std::vector<Item>& Item::getList() {
    static std::vector<Item> list = {
        Item(Text::get("ITEM_01"), Gradient(Color::Brown), 0x43, true, Text::get("METHOD_SIRLOIN"), nullptr, VersionBits::ALL),
        Item(Text::get("ITEM_02"), Gradient(Color::Silver), 0x3EE, true, Text::get("METHOD_TICKET"), Cheat::j3Transfer, VersionBits::J3P_ALL),
        Item(Text::get("ITEM_03"), Gradient(Color(0x728BD4FF), Color(0xD4D4D4FF)), 0x3E3, true, Text::get("METHOD_STATUE_TERRY"), Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item(Text::get("ITEM_04"), Gradient(Color(0x728BD4FF), Color(0xD4D4D4FF)), 0x3E4, true, Text::get("METHOD_STATUE_MILLY"), Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item(Text::get("ITEM_05"), Gradient(Color(0xE688C4FF), Color(0xF29C84FF)), 0x3E5, true, Text::get("METHOD_STATUE_TARA"), Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item(Text::get("ITEM_06"), Gradient(Color(0xE688C4FF), Color(0xF29C84FF)), 0x3E6, true, Text::get("METHOD_STATUE_COBI"), Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL)
    };

    return list;
};

Title::Title(const std::string& name,
             u16 id,
             Gradient gradient,
             const std::string& unlockMethod,
             VersionBits version) :
Content(name, id, gradient, unlockMethod, version) {}

std::vector<Title>& Title::getList() {
    static std::vector<Title> list = {
        Title(Text::get("TITLE_01"), 0x79, Gradient::Bronze, Text::get("METHOD_WIFI_BATTLE_1"), VersionBits::ALL),
        Title(Text::get("TITLE_02"), 0x7A, Gradient::Silver, Text::get("METHOD_WIFI_BATTLE_10"), VersionBits::ALL),
        Title(Text::get("TITLE_03"), 0x7B, Gradient::Gold, Text::get("METHOD_WIFI_BATTLE_100"), VersionBits::ALL),
        Title(Text::get("TITLE_04"), 0x7F, Gradient::Bronze, Text::get("METHOD_WIFI_PTS_10"), VersionBits::ALL),
        Title(Text::get("TITLE_05"), 0x80, Gradient::Bronze, Text::get("METHOD_WIFI_PTS_500"), VersionBits::ALL),
        Title(Text::get("TITLE_06"), 0x81, Gradient::Silver, Text::get("METHOD_WIFI_PTS_1000"), VersionBits::ALL),
        Title(Text::get("TITLE_07"), 0x82, Gradient::Silver, Text::get("METHOD_WIFI_PTS_4000"), VersionBits::ALL),
        Title(Text::get("TITLE_08"), 0x83, Gradient::Gold, Text::get("METHOD_WIFI_PTS_5000"), VersionBits::ALL),
        Title(Text::get("TITLE_09"), 0x84, Gradient::Gold, Text::get("METHOD_WIFI_PTS_9999"), VersionBits::ALL),
        Title(Text::get("TITLE_10"), 0x8B, Gradient::Bronze, Text::get("METHOD_STREETPASS_MEET_10"), VersionBits::ALL),
        Title(Text::get("TITLE_11"), 0x8C, Gradient::Silver, Text::get("METHOD_STREETPASS_MEET_100"), VersionBits::ALL),
        Title(Text::get("TITLE_12"), 0x8D, Gradient::Gold, Text::get("METHOD_STREETPASS_MEET_1000"), VersionBits::ALL),
        Title(Text::get("TITLE_13"), 0x8E, Gradient::Bronze, Text::get("METHOD_STREETPASS_WIN_10"), VersionBits::ALL),
        Title(Text::get("TITLE_14"), 0x8F, Gradient::Silver, Text::get("METHOD_STREETPASS_WIN_100"), VersionBits::ALL),
        Title(Text::get("TITLE_15"), 0x90, Gradient::Gold, Text::get("METHOD_STREETPASS_WIN_1000"), VersionBits::ALL),
        Title(Text::get("TITLE_16"), 0x91, Gradient::Bronze, Text::get("METHOD_MGP_PLAY"), VersionBits::ALL),
        Title(Text::get("TITLE_17"), 0x92, Gradient::Bronze, Text::get("METHOD_MGP_10000"), VersionBits::ALL),
        Title(Text::get("TITLE_18"), 0x93, Gradient::Silver, Text::get("METHOD_MGP_1000"), VersionBits::ALL),
        Title(Text::get("TITLE_19"), 0x94, Gradient::Silver, Text::get("METHOD_MGP_100"), VersionBits::ALL),
        Title(Text::get("TITLE_20"), 0x95, Gradient::Gold, Text::get("METHOD_MGP_8"), VersionBits::ALL),
        Title(Text::get("TITLE_21"), 0x96, Gradient::Gold, Text::get("METHOD_MGP_1"), VersionBits::ALL),
        Title(Text::get("TITLE_22"), 0x97, Gradient::Bronze, Text::get("METHOD_CGP_PLAY"), VersionBits::ALL),
        Title(Text::get("TITLE_23"), 0x98, Gradient::Bronze, Text::get("METHOD_CGP_10000"), VersionBits::ALL),
        Title(Text::get("TITLE_24"), 0x99, Gradient::Silver, Text::get("METHOD_CGP_1000"), VersionBits::ALL),
        Title(Text::get("TITLE_25"), 0x9A, Gradient::Silver, Text::get("METHOD_CGP_100"), VersionBits::ALL),
        Title(Text::get("TITLE_26"), 0x9B, Gradient::Gold, Text::get("METHOD_CGP_8"), VersionBits::ALL),
        Title(Text::get("TITLE_27"), 0x9C, Gradient::Gold, Text::get("METHOD_CGP_1"), VersionBits::ALL),
        Title(Text::get("TITLE_28"), 0x9D, Gradient::Bronze, Text::get("METHOD_COMMUNITY_1"), VersionBits::ALL),
        Title(Text::get("TITLE_29"), 0x9E, Gradient::Silver, Text::get("METHOD_COMMUNITY_2"), VersionBits::ALL),
        Title(Text::get("TITLE_30"), 0x9F, Gradient::Gold, Text::get("METHOD_COMMUNITY_3"), VersionBits::ALL)
    };

    return list;
};
