#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "Content.hpp"
#include "GameTitle.hpp"
#include "Memory.hpp"
#include "String.hpp"
using namespace CTRPluginFramework;

Content::Content(const std::string& name, const u16 id, Gradient gradient, const std::string& unlockMethod, VersionBits version)
: name(name), id(id), unlockMethod(unlockMethod), version(version)
{
    // Single color
    if (gradient.power == -1)
        colorName = gradient.first << name;

    // gradient
    else
        colorName = String::gradient(name, gradient.first, gradient.second, gradient.power);
}

Monster::Monster(const std::string& name,
                 const std::string& iconName,
                 Gradient gradient,
                 u16 id,
                 const std::string& unlockMethod,
                 u32 checksum,
                 VersionBits version) :
Content(name, id, gradient, unlockMethod, version),
iconName(iconName), checksum(checksum) {}

std::vector<Monster>& Monster::getList() {
    static std::vector<Monster> list = {
        Monster("Egg-en-ciel", "egg_en_ciel", Gradient(Color::Red, Color::Blue), 0x214, String::downloadableDisk, 0xD1279F38, VersionBits::J3_ALL),
        Monster("Premium Slime", "prem_slime", Gradient(Color(0xFF3AE4FF), Color::White), 0x332, String::downloadableDisk, 0xC0894415, VersionBits::J3P_ALL),
        Monster("Slider Kids", "slider_kids", Gradient(Color(0x0087FFFF), Color(0x4F5757FF)), 0x36B, String::starterTrade, 0, VersionBits::J3P_ALL),
        Monster("Slider Girl", "slider_girl", Gradient(Color(0xF24BBDFF), Color(0x4DC5FEFF), 0.75f), 0x348, String::downloadableDisk, 0xE5C562D3, VersionBits::J3P_ALL),
        Monster("Ultra Slime", "ultra_slime", Gradient(Color(0x0754C0FF), Color(0xE2A400FF), 0.85f), 0x311, String::downloadableDisk, 0xDD87CA2B, VersionBits::J3P_ALL),
        Monster("Nemeslime", "nemeslime", Gradient(Color(0x4F5757FF), Color(0xFFB200FF), 0.85f), 0x312, String::downloadableDisk, 0x13BA6929, VersionBits::J3P_ALL),
        Monster("King Healslime", "king_heal", Gradient(Color(0x0087FFFF), Color::Yellow, 0.95f), 0x1DE, String::spotPass, 0, VersionBits::ALL),
        Monster("Gome-chan", "gomechan", Gradient(Color(0xF0CB00FF), Color(0xFCFA0BFF)), 0x389, String::downloadableDisk, 0x2A168841, VersionBits::J3P_UPD),

        Monster("Dimensional Dragon", "dim_drag", Gradient(Color(0x5AA6AAFF), Color(0x39175BFF)), 0x2CC, String::downloadableDisk, 0x86D6931, VersionBits::J3P_ALL),

        Monster("Halcyon the Godsteed", "horse", Gradient(Color(0xB9BEB9FF), Color(0xB04E1CFF)), 0x371, String::downloadableDisk, 0x69ABD8B1, VersionBits::J3P_ALL),
        Monster("Rebjorn", "poon", Gradient(Color(0xCFA66DFF)), 0x368, String::starterTrade, 0, VersionBits::J3P_ALL),
        
        Monster("Metabode", "metabode", Gradient(Color(0x7000BDFF), Color(0xFBA500FF)), 0x1E1, String::downloadableDisk, 0x3DC73591, VersionBits::J3_ALL),
        Monster("Bebingo Satan", "imp", Gradient(Color(0xFFD5EDFF), Color(0xF3CDB1FF), 0.75f), 0x309, String::streetPass, 0x64764325, VersionBits::J3P_ALL),
        Monster("Chibi One-Twos", "conk_i_ii", Gradient(Color(0xFBEB76FF)), 0x361, String::downloadableDisk, 0xC51B2BCB, VersionBits::J3P_ALL),
        Monster("Chibi Threes", "conk_iii", Gradient(Color(0xFBEB76FF)), 0x362, String::downloadableDisk, 0xEDBB68C3, VersionBits::J3P_ALL),
        Monster("Chibi Fours", "conk_iv", Gradient(Color(0xFBEB76FF)), 0x363, String::downloadableDisk, 0xA6764EF, VersionBits::J3P_ALL),
        Monster("Chibi Eights", "conk_viii", Gradient(Color(0xFBEB76FF)), 0x364, String::downloadableDisk, 0xF6FAA1E6, VersionBits::J3P_ALL),

        Monster("Killer Crimson", "killer_crimson", Gradient(Color(Color::Red), Color(0x575757FF), 0.70f), 0x21E, String::downloadableDisk, 0xEE70D7E9, VersionBits::J3_ALL),
        Monster("Sagittar", "sagittar", Gradient(Color(0x274DE8FF)), 0x339, String::streetPass, 0xCC7A12BA, VersionBits::J3P_ALL),
        Monster("Master CPU Q484", "q484", Gradient(Color(0xDFD410FF), Color(0x545353FF)), 0x35E, String::downloadableDisk, 0x3D606EDA, VersionBits::J3P_ALL),

        Monster("Wulfspade Ace", "wulf_ace", Gradient(Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), 0x1C7, String::downloadableDisk, 0x4FD5E3AC, VersionBits::J3_ALL),
        Monster("Ace of Spades", "ace_spades", Gradient(Color(0x2B4047FF), Color::Red, 0.90f), 0x1C6, String::downloadableDisk, 0x7E36B391, VersionBits::J3_ALL),
        Monster("Wildcard", "wildcard", Gradient(Color(0xEEEEEEFF), Color(0x50D3E0FF), 0.90f), 0x1C5, String::wildcardLeonyx, 0, VersionBits::J3_ALL),
        Monster("Zoma's Little Devil", "zomas_devil", Gradient(Color(0x342C6DFF), Color(0xD87A1BFF)), 0x36A, String::starterTrade, 0, VersionBits::J3P_ALL),
        Monster("Starkers", "starkers", Gradient(Color(0xB95F03FF), Color(0xE49E14FF)), 0x369, String::starterTrade, 0, VersionBits::J3P_ALL),
        Monster("Tiko", "tiko", Gradient(Color(0xFCE411FF), Color::White, 0.80f), 0x2E5, String::downloadableDisk, 0x81C4CBF4, VersionBits::J3P_ALL),
        Monster("Kirlly", "kukri", Gradient(Color(0xF4AACBFF), Color::White, 0.80f), 0x34B, String::downloadableDisk, 0x2D787476, VersionBits::J3P_ALL),
        Monster("God", "numen", Gradient(Color(0xF7BE8BFF), Color::White), 0x2CF, String::downloadableDisk, 0xBBE9EE08, VersionBits::J3P_ALL),
        Monster("Leonyx", "leonyx", Gradient(Color(0xFEC610FF), Color(0xC2976DFF)), 0x1D4, String::wildcardLeonyx, 0, VersionBits::J3_ALL),
        Monster("Leokid", "leokid", Gradient(Color(0xFEC610FF), Color(0xC2976DFF)), 0x342, String::downloadableDisk, 0x1D409934, VersionBits::J3P_ALL),
        Monster("Gold Nochoro Z", "gold_nochoro", Gradient(Color(0xFCAE00FF), Color(0xA58D00FF), 0.80f), 0x228, String::downloadableDisk, 0x13D3A4C6, VersionBits::J3_ALL),
        Monster("Platinum Nochoro Z", "plat_nochoro", Gradient(Color(0xF0F0F0FF), Color(0x858585FF), 0.80f), 0x229, String::downloadableDisk, 0xC72554A, VersionBits::J3_ALL),
        Monster("J3-Rebel Axel", "axel", Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E0, String::j3Transfer, 0, VersionBits::J3P_ALL),
        Monster("J3-Rebel Wing", "wing", Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E3, String::j3Transfer, 0, VersionBits::J3P_ALL),
        Monster("J3-Rebel Marine", "marine", Gradient(Color(0xC5C5B2FF), Color(0x443A3EFF)), 0x1E6, String::j3Transfer, 0, VersionBits::J3P_ALL),
        Monster("Dark Lord Valaat", "valaat", Gradient(Color(0xC81519FF), Color(0xD5BE75FF), 0.40f), 0x1E7, String::spotPassDisk, 0xCEEA5CC2, VersionBits::ALL),
        Monster("Kandar Viking", "kandar_viking", Gradient(Color::Red, Color(0xFFEB0FFF), 0.30f), 0x1E2, String::streetPass50Battles, 0, VersionBits::J3_ALL),
        Monster("Kandar Ladies", "robbin_ladies", Gradient(Color(0x32EEC3FF), Color(0xE30098FF)), 0x34E, String::streetPass50Battles, 0, VersionBits::J3P_ALL),
        Monster("Rugius", "lugius", Gradient(Color(0xF7BE8BFF), Color(0x1D2122FF)), 0x35F, String::downloadableDisk, 0xBF06F924, VersionBits::J3P_ALL),
        Monster("Vera", "vera", Gradient(Color(0x72574CFF), Color(0x1D2122FF)), 0x360, String::downloadableDisk, 0xFCCD4A20, VersionBits::J3P_ALL),
        Monster("Timelord Omega Rolex", "omd_lex", Gradient(Color(0x608795FF), Color(0x382267FF)), 0x326, String::downloadableDisk, 0x4B54D667, VersionBits::J3P_ALL),
        Monster("Aldarahla", "ardalara", Gradient(Color(0x4AC227FF), Color(0x8C39AAFF), 0.40f), 0x333, String::downloadableDisk, 0xB52DD054, VersionBits::J3P_ALL)
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
        // https://wikiwiki.jp/dqdic3rd/%E3%80%90%E3%82%B9%E3%83%A9%E3%82%A4%E3%83%80%E3%83%BC%E3%82%AC%E3%83%BC%E3%83%AB%E3%80%91
        Skill("King SP", Gradient(Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), 0x1F9, 0x375, String::gradient("King SP Book", Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f), String::unlockKingSP, VersionBits::ALL),
        Skill("Demonking SP", Gradient(Color(0x2B4047FF), Color::Red, 0.90f), 0x1FA, 0x376, String::gradient("Demonking SP Book", Color(0x2B4047FF), Color::Red, 0.90f), String::unlockDemonkingSP, VersionBits::J3_ALL),
        Skill("DQ 30th SP", Gradient(Color::Red, Color::White), 0x1FB, 0x377, String::gradient("DQ 30th SP Book", Color::Red, Color::White), String::spotpassBook, VersionBits::J3_ALL),
        Skill("Great East", Gradient(Color::Orange, Color::Lime, 0.80f), 0x1ED, 0x369, String::gradient("Great East Book", Color::Orange, Color::Lime, 0.80f), String::eastStationJ3, VersionBits::ALL),
        Skill("Wonder West", Gradient(Color::DodgerBlue, Color::Orange, 0.20f), 0x1EE, 0x36A, String::gradient("Wonder West Book", Color::DodgerBlue, Color::Orange, 0.20f), String::westStationJ3, VersionBits::ALL),
        Skill("Jolly Eastern Gal", Gradient(ColorPlus::Pink, Color::Lime, 0.80f), 0x241, 0x435, String::gradient("Jolly Eastern Gal Book", ColorPlus::Pink, Color::Lime, 0.80f), String::eastStationJ3P, VersionBits::J3P_ALL),
        Skill("Maiden of the Windy West", Gradient(Color::DodgerBlue, ColorPlus::Pink, 0.20f), 0x242, 0x436, String::gradient("Maiden of the Windy West Book", Color::DodgerBlue, ColorPlus::Pink, 0.20f), String::westStationJ3P, VersionBits::J3P_ALL)
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
        Item("Sirloin Voucher", Gradient(Color::Brown), 0x43, true, String::unlockSirloin, nullptr, VersionBits::ALL),
        Item("Big Book of Accessories", Gradient(Color::Silver), 0x3EE, true, String::transferTicket, Cheat::j3Transfer, VersionBits::J3P_ALL),
        Item("Cottney's Album I", Gradient(Color(0x728BD4FF), Color(0xD4D4D4FF)), 0x3E3, true, String::terryStatue, Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item("Cottney's Album II", Gradient(Color(0x728BD4FF), Color(0xD4D4D4FF)), 0x3E4, true, String::mireilleStatue, Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item("Rottney's Album I", Gradient(Color(0xE688C4FF), Color(0xF29C84FF)), 0x3E5, true, String::iruStatue, Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL),
        Item("Rottney's Album II", Gradient(Color(0xE688C4FF), Color(0xF29C84FF)), 0x3E6, true, String::lucaStatue, Cheat::cottneyRottneyAlbums, VersionBits::J3P_ALL)
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
        Title("Network Rookie", 0x79, Gradient::Bronze, String::wifiBattle1, VersionBits::ALL),
        Title("Network Master", 0x7A, Gradient::Silver, String::wifiBattle10, VersionBits::ALL),
        Title("Network Champ", 0x7B, Gradient::Gold, String::wifiBattle100, VersionBits::ALL),
        Title("Star Warrior", 0x7F, Gradient::Bronze, String::wifiPts10, VersionBits::ALL),
        Title("Nova Star Warrior", 0x80, Gradient::Bronze, String::wifiPts500, VersionBits::ALL),
        Title("Galactic Warrior", 0x81, Gradient::Silver, String::wifiPts1000, VersionBits::ALL),
        Title("Large Galaxy Warrior", 0x82, Gradient::Silver, String::wifiPts4000, VersionBits::ALL),
        Title("Universal Warrior", 0x83, Gradient::Gold, String::wifiPts5000, VersionBits::ALL),
        Title("Cosmic Warrior", 0x84, Gradient::Gold, String::wifiPts9999, VersionBits::ALL),
        Title("Contact Rookie", 0x8B, Gradient::Bronze, String::meetStreetpass10, VersionBits::ALL),
        Title("Contact Veteran", 0x8C, Gradient::Silver, String::meetStreetpass100, VersionBits::ALL),
        Title("Contact Elite", 0x8D, Gradient::Gold, String::meetStreetpass1000, VersionBits::ALL),
        Title("Victory Rookie", 0x8E, Gradient::Bronze, String::winStreetpass10, VersionBits::ALL),
        Title("Victory Veteran", 0x8F, Gradient::Silver, String::winStreetpass100, VersionBits::ALL),
        Title("Victory Elite", 0x90, Gradient::Gold, String::winStreetpass1000, VersionBits::ALL),
        Title("MGP Entry", 0x91, Gradient::Bronze, String::mgpPlay, VersionBits::ALL),
        Title("MGP Top 10K", 0x92, Gradient::Bronze, String::mgp10000, VersionBits::ALL),
        Title("MGP Top 1K", 0x93, Gradient::Silver, String::mgp1000, VersionBits::ALL),
        Title("MGP Top 100", 0x94, Gradient::Silver, String::mgp100, VersionBits::ALL),
        Title("MGP Top 8", 0x95, Gradient::Gold, String::mgp8, VersionBits::ALL),
        Title("MGP Champ", 0x96, Gradient::Gold, String::mgp1, VersionBits::ALL),
        Title("CGP Entry", 0x97, Gradient::Bronze, String::cgpPlay, VersionBits::ALL),
        Title("CGP Top 10K", 0x98, Gradient::Bronze, String::cgp10000, VersionBits::ALL),
        Title("CGP Top 1K", 0x99, Gradient::Silver, String::cgp1000, VersionBits::ALL),
        Title("CGP Top 100", 0x9A, Gradient::Silver, String::cgp100, VersionBits::ALL),
        Title("CGP Top 8", 0x9B, Gradient::Gold, String::cgp8, VersionBits::ALL),
        Title("CGP Champ", 0x9C, Gradient::Gold, String::cgp1, VersionBits::ALL),
        Title("Owner Rookie", 0x9D, Gradient::Bronze, String::commu1, VersionBits::ALL),
        Title("Owner Veteran", 0x9E, Gradient::Silver, String::commu2, VersionBits::ALL),
        Title("Owner Elite", 0x9F, Gradient::Gold, String::commu3, VersionBits::ALL)
    };

    return list;
};
