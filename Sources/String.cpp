#include <CTRPluginFramework.hpp>
#include "String.hpp"
using namespace CTRPluginFramework;

const std::string

// yes no on off
String::yes = String::bold(Color::Lime << "Yes"),
String::no = String::bold(Color::Red << "No"),
String::on = String::bold(Color::Lime << "ON [ー●]"),
String::off = String::bold(Color::Red << "OFF [●ー]"),

// game names
String::dqmj3p = String::gradient("DQMJ3P", Color::Purple, Color::Yellow, 0.75f),
String::dqmj3 = String::gradient("DQMJ3", Color::Purple, Color::Red, 0.75f),

// main 3gx
String::unobtainableContent = Color::Silver << "Unobtainable content",
String::discordLink = Color(0x5865F2FF) << ToggleDrawMode(Render::FontDrawMode::UNDERLINE | Render::FontDrawMode::BOLD) << "discord.gg/W5yRJpDd5e" << ToggleDrawMode(Render::FontDrawMode::UNDERLINE | Render::FontDrawMode::BOLD),
String::pluginNotes = Color::Silver << "A cheat plugin to unlock monsters locked behind online, StreetPass and trading features.\nAllows completing the game to 100% despite discontinued features!\n\nPlugin made by " << Color::Yellow << "Anthcny" << Color::Silver << "\nIf you encounter any issue, please report it on the DQMJ3P Discord server: " << discordLink,
String::welcome = String::gradient("Plugin ready!", Color::Purple, Color::Yellow, 0.75f),
String::pressSelect = "Press Select to open the menu!",
String::homeMenu = Color::Silver << "You will return to the Home Menu.",
String::cantWriteFreeMem = Color::Silver << "Could not write ASM instruction to free CTRPF memory space.\n\nReturn to home menu?",
String::pressBExit = Color::White << "Press \uE001 to exit",

// 3gx cheats
String::cheat_unobtainableContent = String::bold(String::gradient("Unobtainable content", Color::Red, Color::Blue)),
String::notes_unobtainableContent = Color::Silver << "Choose what unobtainable content to unlock.\n\n" << Color::Orange << "NOTES: For monsters, you must enable the cheat. Then head to the WiFi Square and talk to the rightmost NPC. If the cheat is enabled, only unobtainable monsters will appear. You will be able to buy them even if you don't have enough WiFi Coins!",

// global messagebox
String::success = String::bold(Color::Lime << "Success"),
String::info = String::bold(Color::DodgerBlue << "Information"),
String::question = String::bold(Color::DodgerBlue << "Question"),
String::warning = String::bold(Color::Orange << "Warning"),
String::error = String::bold(Color::Red << "Error"),

// invalid game / version messagebox
String::wrongGame = Color::Silver << "This is a plugin for " << String::dqmj3 << Color::Silver << " and " << String::dqmj3p << Color::Silver << "!",
String::unusableVersion = Color::Silver << "It looks like you have a modified version of the game that cannot be used with this plugin.\nIf you think this is a mistake, please continue.\n\nLaunch the plugin anyway?",

// cheats
String::all = String::bold("[ALL]"),
String::unlockedSkills = " been unlocked in the library and the corresponding books have been added in your inventory!",
String::unlockedItems = " been unlocked in the library and added in your inventory!",
String::unlockedTitles = " been unlocked in the library!",
String::monstersCheatON = Color::Silver << "The cheat is now enabled!\nTo unlock the monsters, talk to the rightmost NPC at the WiFi Square. You will be able to buy the unobtainable monsters even if you don't have enough coins to buy them.",
String::monstersCheatONEmuWarning = "\nIf you can't buy monsters despite not having enough coins, restart your game and make sure to enable the cheat BEFORE opening the menu where you buy monsters.",
String::monstersCheatOFF = Color::Silver << "The cheat is now disabled.\nThe original behavior was restored.",
String::howToUnlockMonsters = Color::Silver << "To unlock monsters, go to the WiFi Square and talk to the rightmost NPC to exchange WiFi Coins with monsters!\nThe cheat will allow you to exchange even if you don't have enough coins!\n\nMake sure to enable the cheat first.",
String::sirloinConfirm = Color::Silver << "To get this item, you must have the shop of healing items opened on the item menu.\nThe plugin will attempt to apply a patch so you can buy a " << Color::Brown << "Sirloin Voucher" << Color::Silver << ".\n\nConfirm?",
String::sirloinSuccess = Color::Silver << "The cheat has been applied successfully!\nThe first item of the shop is now a " << Color::Brown << "Sirloin Voucher" << Color::Silver << ". You can buy it for free and trade it for " << Color::Brown << "Sirloin Bites" << Color::Silver << ".\n\nMake sure to refresh the first page to make it shown.",
String::sirloinWait = Color::Silver << "Please wait... Press \uE001 to cancel.",
String::sirloinError = Color::Silver << "The cheat could not work. Make sure you have the shop for healing items opened.",
String::sirloinNotUnlockedAll = Color::Silver << "\n\nNote that " << Color::Brown << "Sirloin Voucher" << Color::Silver << " was not unlocked. Ysou need to select it individually as it requires special computation to be unlocked.",
String::unlockOnceAlreadyObtained1 = Color::Silver << "You already unlocked ",
String::unlockOnceAlreadyObtained2 = Color::Silver << "!\nThis item is obtainable only once.",
String::itemUnlocked = Color::Silver << " has been unlocked in the library and added in your inventory!",

// keyboard
String::chooseOption = String::bold("\nChoose an option!"),
String::chooseCategory = String::bold("\nChoose a category!"),
String::categoryMonsters = String::bold(Color::Red << "Monsters"),
String::categorySkills = String::bold(Color::Orange << "Skills"),
String::categoryItems = String::bold(Color::Yellow << "Items"),
String::categoryTitles = String::bold(Color::Lime << "Titles"),
String::chooseMonster = String::bold("\nChoose a monster!"),
String::chooseSkillUnlock = String::bold("\nChoose a skill to unlock!"),
String::chooseItemUnlock = String::bold("\nChoose an item to unlock!"),
String::chooseTitleUnlock = String::bold("\nChoose a title to unlock!"),
String::unlockMethods = String::bold(Color::Silver << "Unlock methods"),

// unlock methods
String::downloadableDisk = "Downloadable Disks at the Wi-Fi Square.",
String::starterTrade = "- Trade with a player whose starter is this monster\n- Scoutable through StreetPass battles",
String::j3Transfer = "Use the DQMJ3 monster transfer\nfeature after obtaining the monster in DQMJ3.",
String::streetPass = "SQEX Streetpass distributions.",
String::spotPass = "SpotPass distributions, spawned in the sky.",
String::spotPassDisk = String::dqmj3p << Color::Silver << ": SpotPass distributions, spawned in the sky.\n" << String::dqmj3 << Color::Silver << ": " << String::downloadableDisk,
String::spotpassBook = "Held by large SpotPass monsters in the sky.",
String::streetPass50Battles = "Win 50 StreetPass battles.",
String::wildcardLeonyx = "Every 100 won StreetPass battle\nwill either reward " << String::bold(String::gradient("Wildcard", Color(0xEEEEEEFF), Color(0x50D3E0FF), 0.90f)) << Color::Silver << " or " << String::bold(String::gradient("Leonyx", Color(0xFEC610FF), Color(0xC2976DFF))) << Color::Silver << ".",
String::unlockKingSP = String::dqmj3p << Color::Silver << ": Held by " << String::bold(String::gradient("Rugius", Color(0xF7BE8BFF), Color(0x1D2122FF))) << Color::Silver << " (downloadable Disk).\n" << String::dqmj3 << Color::Silver << ": Held by " << String::bold(String::gradient("Wulfspade Ace", Color(0xEEEEEEFF), Color(0xD856FFFF), 0.90f)) << Color::Silver << " (downloadable Disk).",
String::unlockDemonkingSP = "Held by " << String::bold(String::gradient("Ace of Spades", Color(0x2B4047FF), Color::Red, 0.90f)) << Color::Silver << " (downloadable Disk).",
String::eastStationJ3 = "Obtainable from japanese eastern Nintendo 3DS\nStations from DQMJ3: a King Healslime was\ndistributed with the skill.",
String::westStationJ3 = "Obtainable from japanese western Nintendo 3DS\nStations from DQMJ3: a King Healslime was\ndistributed with the skill.",
String::eastStationJ3P = "Obtainable from japanese eastern Nintendo 3DS\nStations: a Slider Girl was distributed with the skill.",
String::westStationJ3P = "Obtainable from japanese western Nintendo 3DS\nStations: a Slider Girl was distributed with the skill.",
String::unlockSirloin = "Input the following Gift Code:\n」ちョＵ使Ｍｏ不ｒＥ憎火鬼花エ剣\nIt was shown in a japanese strategy guide.",
String::transferTicket = "Use the DQMJ3 monster transfer feature and\ninspect the machine in Wook Park behind the\nyellow Fondude.",
String::terryStatue = "Use 30 Masters / Challenges GP tickets and talk to\nthe Terry statue in the Wi-Fi Square.",
String::mireilleStatue = "Win 30 Wi-Fi matches and talk to the Mireille statue\nin the Wi-Fi Square.",
String::iruStatue = "Spend at least 30 Gold and Platinum Coins and\ntalk to the Iru statue in the Wi-Fi Square.",
String::lucaStatue = "Win 30 StreetPass matches and talk to the Luca\nstatue in the Wi-Fi Square.",
String::wifiBattle1 = "Play 1 Wi-Fi battle.",
String::wifiBattle10 = "Play 10 Wi-Fi battles.",
String::wifiBattle100 = "Play 100 Wi-Fi battles.",
String::wifiPts10 = "Gather 10 Wi-Fi points.",
String::wifiPts500 = "Gather 500 Wi-Fi points.",
String::wifiPts1000 = "Gather 1000 Wi-Fi points.",
String::wifiPts4000 = "Gather 4000 Wi-Fi points.",
String::wifiPts5000 = "Gather 5000 Wi-Fi points.",
String::wifiPts9999 = "Gather 9999 Wi-Fi points.",
String::meetStreetpass10 = "Meet 10 people via StreetPass.",
String::meetStreetpass100 = "Meet 100 people via StreetPass.",
String::meetStreetpass1000 = "Meet 1000 people via StreetPass.",
String::winStreetpass10 = "Win 10 StreetPass battles.",
String::winStreetpass100 = "Win 100 StreetPass battles.",
String::winStreetpass1000 = "Win 1000 StreetPass battles.",
String::mgpPlay = "Participate in Masters GP once.",
String::mgp10000 = "Get placed in the top 10,000 in Masters GP.",
String::mgp1000 = "Get placed in the top 1,000 in Masters GP.",
String::mgp100 = "Get placed in the top 100 in Masters GP.",
String::mgp8 = "Get placed in the top 8 in Masters GP.",
String::mgp1 = "Get placed #1 in Masters GP.",
String::cgpPlay = "Participate in Challengers GP once.",
String::cgp10000 = "Get placed in the top 10,000 in Challengers GP.",
String::cgp1000 = "Get placed in the top 1,000 in Challengers GP.",
String::cgp100 = "Get placed in the top 100 in Challengers GP.",
String::cgp8 = "Get placed in the top 8 in Challengers GP.",
String::cgp1 = "Get placed #1 in Challengers GP.",
String::commu1 = "Own a Wi-Fi community with 5 matches played.",
String::commu2 = "Own a Wi-Fi community with 10 players or more\nwith 50 matches played.",
String::commu3 = "Own a Wi-Fi community with 100 players or more\nwith 500 matches played.",

String::empty = "";
