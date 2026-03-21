#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "Content.hpp"
#include "CTRPFExtension.hpp"
#include "Game.hpp"
#include "GameHook.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
#include "String.hpp"
#include <array>
#include <unordered_map>
using namespace CTRPluginFramework;
const Screen &top = OSD::GetTopScreen(),
             &bottom = OSD::GetBottomScreen();

std::vector<CustomIcon> Cheat::monsterIcons;
std::vector<Monster> Cheat::monsters;
std::vector<Skill> Cheat::skills;
std::vector<Item> Cheat::items;
std::vector<Title> Cheat::titles;
std::vector<std::string> Cheat::skillNames, Cheat::itemNames, Cheat::titleNames;
Addr Cheat::_monstersCanBuyAddr = Macro::FREE_RAM_SPACE_ADDR;

void Cheat::init() {
    ProcessPlus::ClearFreeMem();

    const std::string all = String::all;

    // content
    Cheat::monsters = Content::get(Monster::getList());
    Cheat::skills = Content::get(Skill::getList());
    Cheat::items = Content::get(Item::getList());
    Cheat::titles = Content::get(Title::getList());

    // names
    Cheat::skillNames.push_back(all);
    Cheat::itemNames.push_back(all);
    Cheat::titleNames.push_back(all);
    for (Skill& skill : Cheat::skills) Cheat::skillNames.push_back(ToggleDrawMode(Render::BOLD) << skill.colorName << ToggleDrawMode(Render::BOLD));
    for (Item& item : Cheat::items) Cheat::itemNames.push_back(ToggleDrawMode(Render::BOLD) << item.colorName << ToggleDrawMode(Render::BOLD));
    for (Title& title : Cheat::titles) Cheat::titleNames.push_back(ToggleDrawMode(Render::BOLD) << title.colorName << ToggleDrawMode(Render::BOLD));

    // monster icons
    Cheat::monsterIcons.emplace_back(CustomIconPlus::loadIcon("Icons/all.bin"));
    for (Monster& monster : monsters) {
        std::string path = "Icons/" + monster.iconName + ".bin";
        Cheat::monsterIcons.emplace_back(CustomIconPlus::loadIcon(path));
    }

    // Process::Write32(0x28AA70, 0xE1A00000);
    // J3P_UPD: makes bought monsters always valid: monster data built from the loaded checksum is always seen as valid as long as the cheat is enabled
    
    // init patches
    Cheat::_writeMonsterChecksums();
    GameHook::init(Cheat::monsters);
}

void Cheat::unobtainableContent(MenuEntry* entry) {
    Addr addr;
    if (!PTR::getSave(addr))
        return;

    static std::vector<std::string> categories = {
        String::categoryMonsters,
        String::categorySkills,
        String::categoryItems,
        String::categoryTitles
    };

    static std::array<KBChoice (*)(Keyboard&, int, const std::string&), 4> functions = {
        Cheat::giveMonster,
        Cheat::giveSkill,
        Cheat::giveItem,
        Cheat::giveTitle
    };

    int category = 0;
    while (true) {
        Keyboard kb(CenterAlign(String::chooseCategory, 30));
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
        std::vector<std::string> choices = {
            on ? String::on : String::off,
            String::unlockMethods
        };

        kb.GetMessage() = CenterAlign(String::chooseOption, 30);
        kb.Populate(choices);
        kb.ChangeSelectedEntry(choice);
        choice = kb.Open();
    
        if (choice == 0) {
            on = !on;
            Process::Write8(Cheat::_monstersCanBuyAddr, on);
            GameHook::buyMonstersNotEnoughCoins(on);
            GameHook::monstersCanBuy(on);
            GameHook::buyEveryMonsterKind(on);
            MessageBoxPlus::wrap(String::info, on ? (System::IsCitra() ? String::monstersCheatON << String::monstersCheatONEmuWarning : String::monstersCheatON) : String::monstersCheatOFF, DialogType::DialogOk, ClearScreen::Top);
        }

        else if (choice == 1)
            while (Cheat::monstersMenu());
    }
    while (choice >= 0);

    return KBChoice(0, true);
}

bool Cheat::monstersMenu() {
    // params
    static const int ICONS_PER_LINE = 7,
                     ICON_X_START = 37,
                     ICON_Y_START = 32,
                     ICON_GAP = CustomIconPlus::size + 6,
                     ICON_COUNT = Cheat::monsterIcons.size() - 1;

    static int cursor = 0;
    FwkSettings& settings = FwkSettings::Get();

    // draw screens
    top.DrawRect(Macro::SCREEN_TOP_MENU_X, Macro::SCREEN_TOP_MENU_Y, Macro::SCREEN_TOP_MENU_WIDTH, Macro::SCREEN_MENU_HEIGHT, settings.CustomKeyboard.BackgroundMain);
    top.DrawRect(Macro::SCREEN_TOP_MENU_X + 2, Macro::SCREEN_TOP_MENU_Y + 2, Macro::SCREEN_TOP_MENU_WIDTH - 4, Macro::SCREEN_MENU_HEIGHT - 4, settings.CustomKeyboard.BackgroundBorder, false);

    bottom.DrawRect(Macro::SCREEN_BOT_MENU_X, Macro::SCREEN_BOT_MENU_Y, Macro::SCREEN_BOT_MENU_WIDTH, Macro::SCREEN_MENU_HEIGHT, settings.CustomKeyboard.BackgroundMain);
    bottom.DrawRect(Macro::SCREEN_BOT_MENU_X + 2, Macro::SCREEN_BOT_MENU_Y + 2, Macro::SCREEN_BOT_MENU_WIDTH - 4, Macro::SCREEN_MENU_HEIGHT - 4, settings.CustomKeyboard.BackgroundBorder, false);

    // selected outline
    bottom.DrawRect(
        ICON_X_START + (cursor % ICONS_PER_LINE) * ICON_GAP - 1,
        ICON_Y_START + (cursor / ICONS_PER_LINE) * ICON_GAP - 1,
        CustomIconPlus::size + 2,
        CustomIconPlus::size + 2,
        settings.CustomKeyboard.KeyBackgroundPressed
    );

    // draw icons + touch input
    UIntVector touch = Touch::GetPosition();
    for (int i = 0; i < ICON_COUNT; i++) {
        int x = ICON_X_START + (i % ICONS_PER_LINE) * ICON_GAP;
        int y = ICON_Y_START + floor(i / ICONS_PER_LINE) * ICON_GAP;

        CustomIcon& icon = Cheat::monsterIcons[i + 1];
        CustomIconPlus::draw(bottom, icon, x, y);

        const Rect<u32> rect(x, y, CustomIconPlus::size, CustomIconPlus::size);
        if (Touch::IsDown() && rect.Contains(touch.x, touch.y))
            cursor = i;
    }
    
    // top screen desc
    static const int SYSFONT_HEIGHT = 16,
                     PAD = 4;

    KBMessageParts parts = KBCallback::getMessageParts(Cheat::monsters, Offset::get(Offset::LIB_MONSTERS), String::chooseMonster, cursor + 1, false);
    int textYPos = Macro::SCREEN_TOP_MENU_Y + PAD + SYSFONT_HEIGHT;
    int centerPos = String::centerSysFont(parts.title, SCREEN_TOP_MENU_X, Macro::SCREEN_TOP_MENU_WIDTH);
    int regularPos = PAD + SCREEN_TOP_MENU_X;

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

    // exit text
    bottom.DrawSysfont(String::pressBExit, 188, 184);

    // button inputs
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
            String::bold(Cheat::monsters[cursor].colorName),
            String::howToUnlockMonsters
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
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::skills, Offset::get(Offset::LIB_SKILLS), String::chooseSkillUnlock, selectedIdx, selectedIdx == 0).concat();
    
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

        MessageBoxPlus::wrap(String::success,
        Color::Yellow << std::to_string(count) << Color::Silver << " skill" << String::plural(count) << " ha" << (count == 1 ? "s" : "ve") << String::unlockedSkills);
        return KBChoice(0, true);
    }
    else {
        Skill& skill = Cheat::skills[choice - 1];
        bool unlocked = Game::unlock(Offset::get(Offset::LIB_SKILLS), skill.id);
        Game::increaseItemCount(skill.bookId);

        MessageBoxPlus::wrap(String::success,
        (unlocked ? (Color::Yellow << skill.name << Color::Silver << " has been unlocked and\n") : "") << Color::Yellow << "1x " << skill.bookName << Color::Silver << " was added in your inventory!");
        return KBChoice(choice, false);
    }
}

KBChoice Cheat::giveItem(Keyboard& kb, int selectedIdx, const std::string& message) {
    kb.Populate(Cheat::itemNames);
    kb.OnKeyboardEvent(KBCallback::unobtainableContent_items_cb);
    kb.ChangeSelectedEntry(selectedIdx);
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::items, Offset::get(Offset::LIB_ITEMS), String::chooseItemUnlock, selectedIdx, selectedIdx == 0).concat();

    int choice = kb.Open();
    if (choice < 0)
        return KBChoice(-1, true);

    bool all = choice == 0;

    if (all) {
        int count = 0;
        for (Item& item : Cheat::items) {
            // skip special case: Sirloin Voucher
            if (item.id == Cheat::items[0].id)
                continue;

            if (item.unlockableOnce && Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id))
                continue;

            bool unlocked = Game::unlock(Offset::get(Offset::LIB_ITEMS), item.id);
            Game::increaseItemCount(item.id);

            if (unlocked) {
                count++;
                if (item.onUnlock)
                    item.onUnlock(&item);
            }
        }

        MessageBoxPlus::wrap(String::success,
        Color::Yellow << std::to_string(count) << Color::Silver << " item" << String::plural(count) << " ha" << (count == 1 ? "s" : "ve") << String::unlockedItems << String::sirloinNotUnlockedAll);
        return KBChoice(0, true);
    }

    // special case for Sirloin Vouchers
    else if (choice == 1) {
        static const KBChoice kbChoice(1, false);

        Item& item = Cheat::items[0];
        if (Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id)) {
            MessageBoxPlus::wrap(String::error, String::unlockOnceAlreadyObtained1 << item.colorName << Color::Silver << String::unlockOnceAlreadyObtained2);
            return kbChoice;
        }

        bool ans = MessageBoxPlus::wrap(String::question, String::sirloinConfirm, DialogType::DialogYesNo);
        if (!ans)
            return kbChoice;

        // search pattern in memory
        static const Addr start = 0x8000000, end = 0x9000000;
        static const u32 ptrSize = GameTitle::isJ3P() ? 0x898 : 0x800;
        Addr addr = start;
        u32 read32[2];
        u16 read16;
        Clock clock;
        FwkSettings& settings = FwkSettings::Get();

        while (addr < end) {
            if (clock.HasTimePassed(Milliseconds(500))) {
                // draw main frame
                top.DrawRect(Macro::SCREEN_TOP_MENU_X, Macro::SCREEN_TOP_MENU_Y, Macro::SCREEN_TOP_MENU_WIDTH, Macro::SCREEN_MENU_HEIGHT, settings.BackgroundMainColor);
                top.DrawRect(Macro::SCREEN_TOP_MENU_X + 2, Macro::SCREEN_TOP_MENU_Y + 2, Macro::SCREEN_TOP_MENU_WIDTH - 4, Macro::SCREEN_MENU_HEIGHT - 4, settings.BackgroundBorderColor, false);
                top.DrawRect(76, 140, 248, 25, settings.BackgroundBorderColor, false);

                int textPos = String::centerSysFont(String::sirloinWait, 0, Macro::SCREEN_TOP_WIDTH);
                top.DrawSysfont(String::sirloinWait, textPos, 76);

                // progress bar
                float ratio = (float)(addr - start) / (float)(end - start);
                int width = (int)(246 * ratio);
                top.DrawRect(77, 141, width, 23, Color::Lime);
                
                clock.Restart();
                OSD::SwapBuffers();

                Controller::Update();
                if (Controller::IsKeyPressed(Key::B))
                    return kbChoice;
            }

            Process::Read32(addr, read32[0]);
            Process::Read32(addr + 4, read32[1]);
            Process::Read16(addr + 0x10, read16);

            if (read32[0] == 0x5544 && read32[1] == ptrSize && read16 == 1) {
                Process::Write16(addr + 0x10, item.id);
                MessageBoxPlus::wrap(String::success, String::sirloinSuccess);
                return kbChoice;
            }

            addr += sizeof(addr);
        }

        MessageBoxPlus::wrap(String::error, String::sirloinError);
        return kbChoice;
    }

    else {
        Item& item = Cheat::items[choice - 1];

        if (item.unlockableOnce && !Game::isUnlocked(Offset::get(Offset::LIB_ITEMS), item.id)) {
            Game::unlock(Offset::get(Offset::LIB_ITEMS), item.id);
            Game::increaseItemCount(item.id);

            if (item.onUnlock)
                item.onUnlock(&item);

            MessageBoxPlus::wrap(String::success, item.colorName << String::itemUnlocked);
        }
        else
            MessageBoxPlus::wrap(String::error, String::unlockOnceAlreadyObtained1 << item.colorName << Color::Silver << String::unlockOnceAlreadyObtained2);

        return KBChoice(choice, false);
    }
}

KBChoice Cheat::giveTitle(Keyboard& kb, int selectedIdx, const std::string& message) {
    kb.Populate(Cheat::titleNames);
    kb.OnKeyboardEvent(KBCallback::unobtainableContent_titles_cb);
    kb.ChangeSelectedEntry(selectedIdx);
    kb.GetMessage() = KBCallback::getMessageParts(Cheat::titles, Offset::get(Offset::LIB_TITLES), String::chooseTitleUnlock, selectedIdx, selectedIdx == 0).concat();

    int choice = kb.Open();
    if (choice < 0)
        return KBChoice(-1, true);

    bool all = choice == 0;

    if (all) {
        int count = 0;
        for (Title& title : Cheat::titles)
            count += Game::unlock(Offset::get(Offset::LIB_TITLES), title.id);

        MessageBoxPlus::wrap(String::success,
        Color::Yellow << std::to_string(count) << Color::Silver << " title" << String::plural(count) << " ha" << (count == 1 ? "s": "ve") << String::unlockedTitles);
        return KBChoice(0, true);
    }
    else {
        Title& title = Cheat::titles[choice - 1];
        if (!Game::isUnlocked(Offset::get(Offset::LIB_TITLES), title.id)) {
            Game::unlock(Offset::get(Offset::LIB_TITLES), title.id);

            MessageBoxPlus::wrap(String::success,
            Color::Silver << "Title " << String::bold(title.name) << Color::Silver << " has been unlocked!");
        }
        else
            MessageBoxPlus::wrap(String::error, Color::Silver << String::unlockOnceAlreadyObtained1 << title.colorName << Color::Silver << "!");

        return KBChoice(choice, false);
    }
}

void Cheat::j3Transfer(Item* item) {
    Addr addr;
    u32 flags;
    PTR::getSave(addr);
    addr += Offset::get(Offset::J3_TRANSFERED_BITS);

    Process::Read32(addr, flags);
    flags |= (1 << 23); // has transfered monster from J3?
    flags |= (1 << 24); // has claimed ticket (and starter) from Wood Park machine?
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

/*void Cheat::_initAnticheats() {
    Addr addr1 = ARM::getAddr(ARM::AddrType::ANTICHEAT1),
         addr2 = ARM::getAddr(ARM::AddrType::ANTICHEAT2),
         addr3 = ARM::getAddr(ARM::AddrType::ANTICHEAT3);

    Process::Write8(addr1 + 3, 0xEA);
    Process::Write8(addr1 + 3 + 0xC, 0xEA);

    Process::Write8(addr2 + 3, 0xEA);
    Process::Write8(addr2 + 3 + 0x10, 0xEA);

    Process::Write8(addr3 + 3, 0xEA);
}*/

void Cheat::_writeMonsterChecksums() {
    Addr addr;
    if (!PTR::getSave(addr))
        return;

    Addr addrIDs = addr + Offset::get(Offset::DISC_CLEARED_MONSTER_ID),
         addrChecksums = addr + Offset::get(Offset::DISC_CLEARED_MONSTER_CHECKSUM);

    u32 count = 0;

    for (Monster& monster : Cheat::monsters) {
        // skip if checksum is useless
        if (monster.checksum == 0)
            continue;

        // check if enough remaining slots
        if (count == (GameTitle::isJ3() ? 20 : 40)) {
            OSD::Notify(Color::Red << "ERROR " << Color::Silver << "Checksum slot count exceeded");
            return;
        }

        Process::Write16(addrIDs, monster.id);
        Process::Write32(addrChecksums, monster.checksum);

        addrIDs += sizeof(monster.id);
        addrChecksums += sizeof(monster.checksum);
        count++;
    }
}

bool KBCallback::parseEvent(KeyboardEvent& event, int& choice, bool& all) {
    if (event.type != KeyboardEvent::EventType::SelectionChanged && event.type)
        return false;

    // event is triggered if scrolling with touch screen (glitch?)
    if (event.selectedIndex == -1)
        return false;

    choice = event.selectedIndex;
    all = choice == 0;

    return true;
}

void KBCallback::unobtainableContent_monsters_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::monsters, Offset::get(Offset::LIB_MONSTERS), String::chooseMonster);
}

void KBCallback::unobtainableContent_skill_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::skills, Offset::get(Offset::LIB_SKILLS), String::chooseSkillUnlock);
}

void KBCallback::unobtainableContent_items_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::items, Offset::get(Offset::LIB_ITEMS), String::chooseItemUnlock);
}

void KBCallback::unobtainableContent_titles_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::titles, Offset::get(Offset::LIB_TITLES), String::chooseTitleUnlock);
}
