#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "Content.hpp"
#include "CTRPFExtension.hpp"
#include "Game.hpp"
#include "GameHook.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
#include "Settings.hpp"
#include "Text.hpp"
#include <array>
#include <unordered_map>
using namespace CTRPluginFramework;
const Screen &top = OSD::GetTopScreen(),
             &bottom = OSD::GetBottomScreen();

std::vector<CustomIcon> Cheat::monsterIcons, Cheat::coinIcons;
std::vector<Monster> Cheat::monsters;
std::vector<Skill> Cheat::skills;
std::vector<Item> Cheat::items;
std::vector<Title> Cheat::titles;
std::vector<std::string> Cheat::skillNames, Cheat::itemNames, Cheat::titleNames;
std::vector<CustomIcon> Cheat::flagIcons;
Addr Cheat::_monstersCanBuyAddr = Macro::FREE_RAM_SPACE_ADDR;

void Cheat::init(std::vector<CustomIcon::Pixel>& iconsPixels, std::vector<CustomIcon>& flagIcons) {
    ProcessPlus::ClearFreeMem();

    // Content data
    Cheat::monsters = Content::get(Monster::getList());
    Cheat::skills = Content::get(Skill::getList());
    Cheat::items = Content::get(Item::getList());
    Cheat::titles = Content::get(Title::getList());

    // Content texts
    Text::loadContentTexts(Cheat::skills, Cheat::skillNames, Cheat::items, Cheat::itemNames, Cheat::titles, Cheat::titleNames);

    // Monster icons
    for (Monster& monster : monsters)
    Cheat::monsterIcons.emplace_back(monster.getIcon(iconsPixels));

    // Coin icons
    static std::array<u32, 4> coinIconIdxs = {
        42, // Bronze
        43, // Silver
        44, // Gold
        45 // Platinum
    };
    for (u32 idx : coinIconIdxs)
        Cheat::coinIcons.emplace_back(CustomIconPlus::loadIcon(iconsPixels, idx));

    // Flag icons
    Cheat::flagIcons = std::move(flagIcons);

    // Write hooks and jumps to them by default
    GameHook::init(Cheat::monsters);

    // Monster checksums
    if (!Cheat::_writeMonsterChecksums())
        ProcessPlus::Exit();
}

void Cheat::unobtainableContent(MenuEntry* entry) {
    Addr addr;
    if (!PTR::getSave(addr))
        return;

    std::vector<std::string> categories = {
        Text::bold(Text::get("KB_MONSTERS", Color::Red)),
        Text::bold(Text::get("KB_SKILLS", Color::Orange)),
        Text::bold(Text::get("KB_ITEMS", Color::Yellow)),
        Text::bold(Text::get("KB_TITLES", Color::Lime))
    };

    static std::array<KBChoice (*)(Keyboard&, int, const std::string&), 4> functions = {
        Cheat::giveMonster,
        Cheat::giveSkill,
        Cheat::giveItem,
        Cheat::giveTitle
    };

    int category = 0;
    while (true) {
        Keyboard kb(CenterAlign(Text::bold(Text::get("KB_CATEGORY")), 30));
        kb.Populate(categories);
        kb.ChangeSelectedEntry(category);
    
        category = kb.Open();
        if (category < 0) // B on category menu
            return;

        KBChoice choice;
        do {
            choice = functions[category](kb, choice.idx, choice.saveKbMessage);
            choice.saveKbMessage = kb.GetMessage();
        }
        while (!choice.exit);
    }
}

KBChoice Cheat::giveMonster(Keyboard& kb, int selectedIdx, const std::string& message) {
    static bool on = false;
    int choice = 0;

    do {
        std::string choiceOn = Text::bold(Text::get("CHOICE_MONSTERS_ON", Color::Lime)),
                    choiceOff = Text::bold(Text::get("CHOICE_MONSTERS_OFF", Color::Red));
        std::vector<std::string> choices = {
            on ? choiceOn : choiceOff,
            Text::bold(Text::get("KB_UNLOCK_METHODS"))
        };

        kb.GetMessage() = CenterAlign(Text::bold(Text::get("KB_OPTION")), 30);
        kb.Populate(choices);
        kb.ChangeSelectedEntry(choice);
        choice = kb.Open();
    
        if (choice == 0) {
            on = !on;
            Process::Write8(Cheat::_monstersCanBuyAddr, on);

            std::string text = Text::get(on ? "POPUP_MONSTERS_ON" : "POPUP_MONSTERS_OFF", Color::Silver);
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_INFO", Color::DodgerBlue)), text, DialogType::DialogOk, ClearScreen::Top);
        }

        else if (choice == 1)
            while (Cheat::monstersMenu());
    }
    while (choice >= 0);

    return KBChoice(0, true);
}

bool Cheat::monstersMenu() {
    // Params
    static const int ICONS_PER_LINE = 7,
                     ICON_X_START = 37,
                     ICON_Y_START = 32,
                     ICON_GAP = CustomIconPlus::size + 6,
                     ICON_COUNT = Cheat::monsterIcons.size();

    static int cursor = 0;
    FwkSettings& settings = FwkSettings::Get();

    // Draw screens
    top.DrawRect(Macro::SCREEN_TOP_MENU_X, Macro::SCREEN_TOP_MENU_Y, Macro::SCREEN_TOP_MENU_WIDTH, Macro::SCREEN_MENU_HEIGHT, settings.CustomKeyboard.BackgroundMain);
    top.DrawRect(Macro::SCREEN_TOP_MENU_X + 2, Macro::SCREEN_TOP_MENU_Y + 2, Macro::SCREEN_TOP_MENU_WIDTH - 4, Macro::SCREEN_MENU_HEIGHT - 4, settings.CustomKeyboard.BackgroundBorder, false);

    bottom.DrawRect(Macro::SCREEN_BOT_MENU_X, Macro::SCREEN_BOT_MENU_Y, Macro::SCREEN_BOT_MENU_WIDTH, Macro::SCREEN_MENU_HEIGHT, settings.CustomKeyboard.BackgroundMain);
    bottom.DrawRect(Macro::SCREEN_BOT_MENU_X + 2, Macro::SCREEN_BOT_MENU_Y + 2, Macro::SCREEN_BOT_MENU_WIDTH - 4, Macro::SCREEN_MENU_HEIGHT - 4, settings.CustomKeyboard.BackgroundBorder, false);

    // Selected outline
    bottom.DrawRect(
        ICON_X_START + (cursor % ICONS_PER_LINE) * ICON_GAP - 1,
        ICON_Y_START + (cursor / ICONS_PER_LINE) * ICON_GAP - 1,
        CustomIconPlus::size + 2,
        CustomIconPlus::size + 2,
        settings.CustomKeyboard.KeyBackgroundPressed
    );

    // Draw icons + touch input
    UIntVector touch = Touch::GetPosition();
    for (int i = 0; i < ICON_COUNT; i++) {
        int x = ICON_X_START + (i % ICONS_PER_LINE) * ICON_GAP;
        int y = ICON_Y_START + floor(i / ICONS_PER_LINE) * ICON_GAP;

        CustomIcon& icon = Cheat::monsterIcons[i];
        CustomIconPlus::draw(bottom, icon, x, y);

        const Rect<u32> rect(x, y, CustomIconPlus::size, CustomIconPlus::size);
        if (Touch::IsDown() && rect.Contains(touch.x, touch.y))
            cursor = i;
    }
    
    // Top screen desc
    static const int SYSFONT_HEIGHT = 16,
                     PAD = 4;

    KBMessageParts parts = KBCallback::getMessageParts(Cheat::monsters, Offset::get(Offset::LIB_MONSTERS), Text::bold(Text::get("KB_CHOOSE_MONSTER", ColorPlus::None, Color::White)), cursor + 1, false);
    int textYPos = Macro::SCREEN_TOP_MENU_Y + PAD + SYSFONT_HEIGHT;
    int centerPos = Text::centerSysFont(parts.title, Macro::SCREEN_TOP_MENU_X, Macro::SCREEN_TOP_MENU_WIDTH);
    int regularPos = PAD + Macro::SCREEN_TOP_MENU_X;

    top.DrawSysfont(parts.title, centerPos, textYPos);
    textYPos += SYSFONT_HEIGHT * 2;

    for (const std::string& line : parts.how) {
        top.DrawSysfont(line, regularPos, textYPos);
        textYPos += SYSFONT_HEIGHT;
    }
    textYPos += SYSFONT_HEIGHT;

    top.DrawSysfont(parts.unlocked, regularPos, textYPos);
    textYPos += SYSFONT_HEIGHT;

    top.DrawSysfont(parts.storage, regularPos, textYPos);

    // Exit text
    std::string exitText = Text::get("PLUGIN_B_TO_EXIT");
    int exitStartX, exitWidth, exitY;

    if (GameTitle::isJ3P()) {
        exitY = 184;
        exitStartX = GameTitle::isUpdate() ? 181 : 145;
        exitWidth = GameTitle::isUpdate() ? 102 : 138;
    }
    else {
        exitY = 144;
        exitStartX = 0;
        exitWidth = Macro::SCREEN_BOT_WIDTH;
    }

    int exitPos = Text::centerSysFont(exitText, exitStartX, exitWidth);
    bottom.DrawSysfont(exitText, exitPos, exitY);

    // Button inputs
    static const int INPUT_SPEED = 4 * (1 + !System::IsCitra());
    static std::unordered_map<Key, int> inputFrames = {
        {Key::Up, -1},
        {Key::Down, -1},
        {Key::Left, -1},
        {Key::Right, -1}
    };
    Controller::Update();

    if (Controller::IsKeyPressed(Key::A))
        MessageBoxPlus::wrap(
            Text::bold(Cheat::monsters[cursor].colorName),
            Text::get("POPUP_MONSTERS_HOW_TO_UNOCK", Color::Silver)
        );

    if (Controller::IsKeyPressed(Key::B))
        return false;

    if (Controller::IsKeyDown(Key::Up)) {
        int& frames = inputFrames[Key::Up];
        frames++;

        if (frames % INPUT_SPEED == 0)
            if (cursor >= ICONS_PER_LINE)
                cursor = std::max(0, cursor - ICONS_PER_LINE);
    }
    else
        inputFrames[Key::Up] = -1;

    if (Controller::IsKeyDown(Key::Down)) {
        int& frames = inputFrames[Key::Down];
        frames++;

        if (frames % INPUT_SPEED == 0) 
            if (cursor + ICONS_PER_LINE < ICON_COUNT)
                cursor += ICONS_PER_LINE;
    }
    else
        inputFrames[Key::Down] = -1;

    if (Controller::IsKeyDown(Key::Left)) {
        int& frames = inputFrames[Key::Left];
        frames++;

        if (frames % INPUT_SPEED == 0) {
            if (cursor % ICONS_PER_LINE == 0)
                cursor = std::min(ICON_COUNT - 1, --cursor + ICONS_PER_LINE);
            else
                cursor = std::max(0, --cursor);
        }
    }
    else
        inputFrames[Key::Left] = -1;

    if (Controller::IsKeyDown(Key::Right)) {
        int& frames = inputFrames[Key::Right];
        frames++;

        if (frames % INPUT_SPEED == 0) {
            if (cursor % ICONS_PER_LINE == ICONS_PER_LINE - 1)
                cursor -= (ICONS_PER_LINE - 1);
            else
                cursor = std::min(ICON_COUNT - 1, ++cursor);
        }
    }
    else
        inputFrames[Key::Right] = -1;

    OSD::SwapBuffers();
    return true;
}

KBChoice Cheat::giveSkill(Keyboard& kb, int selectedIdx, const std::string& message) {
    kb.Populate(Cheat::skillNames);
    kb.OnKeyboardEvent(KBCallback::unobtainableContent_skill_cb);
    kb.ChangeSelectedEntry(selectedIdx);
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::skills, Offset::get(Offset::LIB_SKILLS), Text::bold(Text::get("KB_CHOOSE_SKILL")), selectedIdx, selectedIdx == 0).concat();
    
    int choice = kb.Open();
    if (choice < 0)
        return KBChoice(-1, true);

    bool all = choice == 0;

    if (all) {
        int count = 0;
        for (Skill& skill : Cheat::skills) {
            count += Game::unlock(Offset::get(Offset::LIB_SKILLS), skill.id);
            Game::increaseItemCount(skill.bookId);
        }

        MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Color::Silver << Text::get("POPUP_UNLOCK_SKILL_ALL", {Color::Yellow << std::to_string(count) << Color::Silver}));
        return KBChoice(0, true);
    }
    else {
        Skill& skill = Cheat::skills[choice - 1];
        bool unlocked = Game::unlock(Offset::get(Offset::LIB_SKILLS), skill.id);
        Game::increaseItemCount(skill.bookId);

        MessageBoxPlus::wrap(
            Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)),
            (unlocked ? Text::get("POPUP_UNLOCK_SKILL_SINGLE_BOOK", Color::Silver, ColorPlus::None, {skill.colorName << Color::Silver, skill.bookName << Color::Silver}) : Text::get("POPUP_UNLOCK_SKILL_SINGLE_BOOK_ONLY", Color::Silver, ColorPlus::None, {skill.bookName << Color::Silver}))
        );
        return KBChoice(choice, false);
    }
}

KBChoice Cheat::giveItem(Keyboard& kb, int selectedIdx, const std::string& message) {
    kb.Populate(Cheat::itemNames);
    kb.OnKeyboardEvent(KBCallback::unobtainableContent_items_cb);
    kb.ChangeSelectedEntry(selectedIdx);
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::items, Offset::get(Offset::LIB_ITEMS), Text::bold(Text::get("KB_CHOOSE_ITEM")), selectedIdx, selectedIdx == 0).concat();

    static Addr giveItemFunc = ARM::getGameFunc(ARM::FuncType::GIVE_ITEM);
    static Addr invMgrAddr;
    if (!PTR::getInvManager(invMgrAddr))
        return KBChoice(-1, true);

    int choice = kb.Open();
    if (choice < 0)
        return KBChoice(-1, true);

    bool all = choice == 0;

    if (all) {
        int count = 0;
        for (Item& item : Cheat::items) {
            if (item.unlockableOnce && Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id))
                continue;

            bool unlocked = !Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id);
            ARM::call<u32>(giveItemFunc, invMgrAddr, item.id, 1);

            if (unlocked) {
                count++;
                if (item.onUnlock)
                    item.onUnlock(&item);
            }
        }

        MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Color::Silver << Text::get("POPUP_UNLOCK_ITEM_ALL", {Color::Yellow << std::to_string(count) << Color::Silver, Text::get("HELPER_SIRLOIN_VOUCHER", Color::Brown, Color::Silver)}));
        return KBChoice(0, true);
    }

    else {
        Item& item = Cheat::items[choice - 1];

        if (item.unlockableOnce && !Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id)) {
            ARM::call<u32>(giveItemFunc, invMgrAddr, item.id, 1);

            if (item.onUnlock)
                item.onUnlock(&item);

            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Text::get("POPUP_UNLOCK_ITEM_SINGLE", {item.colorName << Color::Silver}));
        }
        else
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_ERROR", Color::Red)), Text::get("POPUP_UNLOCK_ONCE_ALREADY_UNLOCKED", Color::Silver, ColorPlus::None, {item.colorName << Color::Silver}));

        return KBChoice(choice, false);
    }
}

KBChoice Cheat::giveTitle(Keyboard& kb, int selectedIdx, const std::string& message) {
    kb.Populate(Cheat::titleNames);
    kb.OnKeyboardEvent(KBCallback::unobtainableContent_titles_cb);
    kb.ChangeSelectedEntry(selectedIdx);
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::titles, Offset::get(Offset::LIB_TITLES), Text::bold(Text::get("KB_CHOOSE_TITLE")), selectedIdx, selectedIdx == 0).concat();

    static Addr giveTitleFunc = ARM::getGameFunc(ARM::FuncType::GIVE_TITLE);
    static Addr titleMgrAddr;
    if (!PTR::getTitleManager(titleMgrAddr))
        return KBChoice(-1, true);

    int choice = kb.Open();
    if (choice < 0)
        return KBChoice(-1, true);

    bool all = choice == 0;

    if (all) {
        int count = 0;
        for (Title& title : Cheat::titles) {
            count += Game::unlock(Offset::get(Offset::LIB_TITLES), title.id);
            ARM::call<u32>(giveTitleFunc, titleMgrAddr, title.id, 1);
        }

        MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Color::Silver << Text::get("POPUP_UNLOCK_TITLE_ALL", {Color::Yellow << std::to_string(count) << Color::Silver}));
        return KBChoice(0, true);
    }
    else {
        Title& title = Cheat::titles[choice - 1];
        if (!Game::isUnlocked(Offset::get(Offset::LIB_TITLES), title.id)) {
            Game::unlock(Offset::get(Offset::LIB_TITLES), title.id);
            u32 res = ARM::call<u32>(giveTitleFunc, titleMgrAddr, title.id, 1);
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Text::get("POPUP_UNLOCK_TITLE_SINGLE", Color::Silver, ColorPlus::None, {title.colorName << Color::Silver}));
        }
        else
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_ERROR", Color::Red)), Text::get("POPUP_UNLOCK_TITLE_SINGLE_ALREADY_UNLOCKED", Color::Silver, ColorPlus::None, {title.colorName << Color::Silver}));

        return KBChoice(choice, false);
    }
}

void Cheat::j3Transfer(Item* item) {
    Addr addr;
    u32 flags;
    PTR::getSave(addr);
    addr += Offset::get(Offset::J3_TRANSFERED_BITS);

    Process::Read32(addr, flags);
    flags |= (1 << 23); // Has transfered monster from J3?
    flags |= (1 << 24); // Has claimed ticket (and starter) from Wood Park machine?
    Process::Write32(addr, flags);
}

void Cheat::cottneyRottneyAlbums(Item* item) {
    static std::unordered_map<u16, u32> statueBits =  {
        {0x3E3, 1 << 14}, // Cottney 1
        {0x3E4, 1 << 15}, // Cottney 2
        {0x3E5, 1 << 16}, // Rottney 1
        {0x3E6, 1 << 17}, // Rottney 2
    };

    if (!item || !statueBits.contains(item->id))
        return;

    Addr addr;
    u32 flags;
    PTR::getSave(addr);
    addr += Offset::get(Offset::WIFI_STATUES_BITS);

    u32 statueBit = statueBits[item->id];
    Process::Read32(addr, flags);
    flags |= statueBit;
    Process::Write32(addr, flags);
}

bool Cheat::isMonsterInStorage(u16 monsterId) {
    Addr addr;
    if (!PTR::getSave(addr))
        return false;

    addr += Offset::get(Offset::STORAGE_DATA);
    u16 id;

    for (int i = 0; i < Macro::MAX_MONSTER_STORAGE_SPACE; i++) {
        Process::Read16(addr + Offset::get(Offset::MSTR_ID), id);

        if (id == monsterId)
            return true;

        addr += Offset::get(Offset::MONSTER_DATA_SIZE);
    }

    return false;
}

bool Cheat::_writeMonsterChecksums() {
    Addr addr;
    if (!PTR::getSave(addr))
        return false;

    Addr addrIDs = addr + Offset::get(Offset::DISC_CLEARED_MONSTER_ID),
         addrChecksums = addr + Offset::get(Offset::DISC_CLEARED_MONSTER_CHECKSUM);

    u32 count = 0;

    for (Monster& monster : Cheat::monsters) {
        // Skip if checksum is useless
        if (monster.checksum == 0)
            continue;

        // Check if enough remaining slots
        if (count == (GameTitle::isJ3() ? Macro::CHECKSUM_SLOTS_J3 : Macro::CHECKSUM_SLOTS_J3P)) {
            FwkSettings settings = FwkSettings::Get();
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_ERROR", Color::Red)), Text::get("MONSTER_CHECKSUM_SLOT_COUNT_EXEEDED", Color::Silver, ColorPlus::None, {Text::getUniversal("DISCORD_URL")}));
            return false;
        }

        Process::Write16(addrIDs, monster.id);
        Process::Write32(addrChecksums, monster.checksum);

        addrIDs += sizeof(monster.id);
        addrChecksums += sizeof(monster.checksum);
        count++;
    }

    return true;
}

void Cheat::wiFiCoins(MenuEntry* entry) {
    Addr addr;
    if (!PTR::getSave(addr))
        return;

    std::array<std::string, 4> coinNames = {
        Text::gradient(Text::get("COIN_BRONZE"), Gradient::Bronze.first, Gradient::Bronze.second, Gradient::Bronze.power),
        Text::gradient(Text::get("COIN_SILVER"), Gradient::Silver.first, Gradient::Silver.second, Gradient::Silver.power),
        Text::gradient(Text::get("COIN_GOLD"), Gradient::Gold.first, Gradient::Gold.second, Gradient::Gold.power),
        Text::gradient(Text::get("COIN_PLATINUM"), Gradient::Platinum.first, Gradient::Platinum.second, Gradient::Platinum.power)
    };

    while (true) {
        Keyboard kb(CenterAlign(Text::bold(Text::get("KB_COINS_GET"))));
        kb.Populate(Cheat::coinIcons);
        kb.IsHexadecimal(false);
        kb.SetMaxLength(3);

        int choice = kb.Open();
        if (choice < 0)
            return;
    
        u16 coins;
        PTR::getSave(addr);
        addr += (Offset::get(Offset::WIFI_COINS) + choice * sizeof(coins));
        Process::Read16(addr, coins);
    
        kb.GetMessage() = CenterAlign(Text::bold(Text::get("KB_COINS_AMOUNT", {coinNames[choice] << Color::White})));
        if (kb.Open(coins, coins) >= 0) {
            coins = std::min(coins, static_cast<u16>(Macro::MAX_ITEM_COUNT));
            u32 res = Cheat::setWiFiCoinsAmount(choice, coins);
        }
    }
}

u32 Cheat::setWiFiCoinsAmount(u8 coinidx, u16 targetAmount) {
    Addr addr;
    if (!PTR::getSave(addr))
        return 0;

    // Get current amount
    addr += Offset::get(Offset::WIFI_COINS) + coinidx * sizeof(targetAmount);
    u16 currentAmount;
    Process::Read16(addr, currentAmount);

    // Determine how much to award
    u16 rewardAmount = targetAmount - currentAmount;

    return ARM::call<u32>(ARM::getGameFunc(ARM::FuncType::REWARD_WIFI_COINS), ARM::UNDEF, coinidx, rewardAmount);
}

void Cheat::switchLanguage(MenuEntry* entry) {
    Keyboard kb(CenterAlign(Text::getChooseLanguageText(Text::language)));
    kb.Populate(Cheat::flagIcons);
    
    int choice = kb.Open();
    if (choice < 0)
        return;

    LanguageId language = Text::getLanguageByIndex(choice);

    // Load texts
    if (!Text::tryLoad(language))
        return;
    
    Text::language = language;
    Settings::set("language", std::to_string(choice), true);
    MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_SUCCESS", Color::Lime)), Text::get("PLUGIN_LANGUAGE_SWITCHED", Color::Silver));
}

bool KBCallback::parseEvent(KeyboardEvent& event, int& choice, bool& all) {
    if (event.type != KeyboardEvent::EventType::SelectionChanged && event.type)
        return false;

    // Event is triggered if scrolling with touch screen (glitch?)
    if (event.selectedIndex == -1)
        return false;

    choice = event.selectedIndex;
    all = choice == 0;

    return true;
}

void KBCallback::unobtainableContent_skill_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::skills, Offset::get(Offset::LIB_SKILLS), Text::bold(Text::get("KB_CHOOSE_SKILL", ColorPlus::None, Color::White)));
}

void KBCallback::unobtainableContent_items_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::items, Offset::get(Offset::LIB_ITEMS), Text::bold(Text::get("KB_CHOOSE_ITEM", ColorPlus::None, Color::White)));
}

void KBCallback::unobtainableContent_titles_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::titles, Offset::get(Offset::LIB_TITLES), Text::bold(Text::get("KB_CHOOSE_TITLE", ColorPlus::None, Color::White)));
}