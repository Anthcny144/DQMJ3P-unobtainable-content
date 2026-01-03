#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "CTRPFExtension.hpp"
#include "Game.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
#include "Storage.hpp"
#include "String.hpp"
#include <array>
#include <type_traits>
#include <unordered_map>
#include <vector>
using namespace CTRPluginFramework;

std::vector<CustomIcon> Cheat::monsterIcons, Cheat::coinIcons;
std::vector<Monster> Cheat::monsters;
std::vector<Skill> Cheat::skills;
std::vector<Item> Cheat::items;
std::vector<Title> Cheat::titles;
std::vector<std::string> Cheat::skillNames, Cheat::itemNames, Cheat::titleNames;

void Cheat::init() {
    const std::string all = String::all;

    // monster data
    if (Cheat::monsters.empty())
        Cheat::monsters = Monster::get();

    // skill data
    if (Cheat::skills.empty())
        Cheat::skills = Skill::get();

    // item data
    if (Cheat::items.empty())
        Cheat::items = Item::get();

    // title data
    if (Cheat::titles.empty())
        Cheat::titles = Title::get();

    // content names
    if (Cheat::skillNames.empty() && Cheat::itemNames.empty() && Cheat::titleNames.empty()) {
        Cheat::skillNames.push_back(all);
        Cheat::itemNames.push_back(all);
        Cheat::titleNames.push_back(all);
        for (Skill& skill : Cheat::skills) Cheat::skillNames.push_back(skill.name);
        for (Item& item : Cheat::items) Cheat::itemNames.push_back(item.name);
        for (Title& title : Cheat::titles) Cheat::titleNames.push_back(title.name);
    }

    // monster icons
    if (Cheat::monsterIcons.empty()) {
        Cheat::monsterIcons.emplace_back(CustomIconPlus::loadIcon("Icons/all.bin"));

        for (Monster& monster : monsters) {
            std::string path = "Icons/" + monster.iconName + ".bin";
            Cheat::monsterIcons.emplace_back(CustomIconPlus::loadIcon(path));
        }
    }

    // wi-fi coin icons
    if (Cheat::coinIcons.empty()) {
        static std::array<std::string, 4> coinNames = {"bron", "silv", "gold", "plat"};

        for (std::string& coinName : coinNames) {
            std::string path = "Icons/coin_" + coinName + ".bin";
            Cheat::coinIcons.emplace_back(CustomIconPlus::loadIcon(path));
        }
    }

    Cheat::sirloinPatch();
    Cheat::wiFiCoinPatch();
}

void Cheat::unobtainableContent(MenuEntry* entry) {
    u32 addr;
    if (!PTR::get(PTR::Type::SAVE, addr) || Storage::isEmpty()) {
        MessageBoxPlus::wrap(String::error, String::noMonster);
        return;
    }

    static std::vector<std::string> categories = {
        String::categoryMonsters,
        String::categorySkills,
        String::categoryItems,
        String::categoryTitles
    };

    ask:
    Keyboard kb(CenterAlign(String::chooseCategory));
    kb.Populate(categories);

    int category = kb.Open(), choice;
    if (category < 0)
        return;

    // monsters
    if (category == 0) {
        askMonster:
        kb.GetMessage() = CenterAlign(String::chooseMonsterUnlock);
        kb.Populate(Cheat::monsterIcons);
        kb.OnKeyboardEvent(KBCallback::unobtainableContent_monsters_cb);

        choice = kb.Open();
        if (choice < 0)
            goto ask;

        bool all = choice == 0;

        // check storage space
        u16 checkSize = all ? monsters.size() : 1;
        if (!Storage::isSpaceAvailable(checkSize)) {
            MessageBoxPlus::wrap(String::error, Color::Silver << "You don't have enough space in your storage!\nYou need at least " << Color::Yellow << std::to_string(checkSize) << " slot" << String::plural(checkSize) << Color::Silver << ".");
            return;
        }

        if (all) {
            size_t count = 0;
            for (Monster& monster : monsters)
                count += Storage::addMonster(monster);

            MessageBoxPlus::wrap(String::success,
            Color::Silver << "All " << Color::Yellow << std::to_string(monsters.size()) << Color::Silver << " monsters have been added to your storage and " << Color::Yellow << std::to_string(count) << Color::Silver << " of them were unlocked in the library!\n\n" << Cheat::invalidMonstersWarning(false));
            goto askMonster;
        }
        else {
            Monster& monster = monsters[choice - 1];
            bool unlocked = Storage::addMonster(monster);
            MessageBoxPlus::wrap(String::success,
            monster.name << Color::Silver << " has been added to your storage" << (unlocked ? " and unlocked in the library" : "") << "!\n\n" << Cheat::invalidMonstersWarning(true));
            goto askMonster;
        }
    }

    // skills
    else if (category == 1) {
        askSkill:
        kb.GetMessage() = CenterAlign(String::chooseSkillUnlock);
        kb.Populate(Cheat::skillNames);
        kb.OnKeyboardEvent(KBCallback::unobtainableContent_skill_cb);
        
        choice = kb.Open();
        if (choice < 0)
            goto ask;

        bool all = choice == 0;

        if (all) {
            size_t count = 0;
            for (Skill& skill : Cheat::skills) {
                count += Game::unlock(Offset::LIB_SKILLS, skill.id);
                Game::increaseItemCount(skill.bookId);
            }

            MessageBoxPlus::wrap(String::success,
            Color::Yellow << std::to_string(count) << Color::Silver << " skill" << String::plural(count) << " ha" << (count == 1 ? "s" : "ve") << String::unlockedSkills);
            goto ask;
        }
        else {
            Skill& skill = Cheat::skills[choice - 1];
            bool unlocked = Game::unlock(Offset::LIB_SKILLS, skill.id);
            Game::increaseItemCount(skill.bookId);

            MessageBoxPlus::wrap(String::success,
            (unlocked ? (Color::Yellow << skill.name << Color::Silver << " has been unlocked and\n") : "") << Color::Yellow << "1x " << skill.bookName << Color::Silver << " was added in your inventory!");
            goto askSkill;
        }
    }

    // items
    else if (category == 2) {
        askItem:
        kb.GetMessage() = CenterAlign(String::chooseItemUnlock);
        kb.Populate(Cheat::itemNames);
        kb.OnKeyboardEvent(KBCallback::unobtainableContent_items_cb);

        choice = kb.Open();
        if (choice < 0)
            goto ask;

        bool all = choice == 0;

        if (all) {
            size_t count = 0;
            for (Item& item : Cheat::items) {
                if (item.unlockableOnce && Game::isUnlocked(Offset::LIB_ITEMS, item.id))
                    continue;

                bool unlocked = Game::unlock(Offset::LIB_ITEMS, item.id);
                Game::increaseItemCount(item.id);

                if (unlocked) {
                    count++;
                    if (item.onUnlock)
                        item.onUnlock(&item);
                }
            }

            MessageBoxPlus::wrap(String::success,
            Color::Yellow << std::to_string(count) << Color::Silver << " item" << String::plural(count) << " ha" << (count == 1 ? "s" : "ve") << String::unlockedItems);
            goto ask;
        }
        else {
            Item& item = Cheat::items[choice - 1];

            if (item.unlockableOnce && !Game::isUnlocked(Offset::LIB_ITEMS, item.id)) {
                Game::unlock(Offset::LIB_ITEMS, item.id);
                Game::increaseItemCount(item.id);

                if (item.onUnlock)
                    item.onUnlock(&item);

                MessageBoxPlus::wrap(String::success, Color::Yellow << item.name << Color::Silver << " has been unlocked in the library and added in your inventory!");
            }
            else
                MessageBoxPlus::wrap(String::error, Color::Silver << "You already unlocked " << item.name << Color::Silver << "!\nThis item is obtainable only once.");

            goto askItem;
        }
    }

    // titles
    else if (category == 3) {
        askTitle:
        kb.GetMessage() = CenterAlign(String::chooseTitleUnlock);
        kb.Populate(Cheat::titleNames);
        kb.OnKeyboardEvent(KBCallback::unobtainableContent_titles_cb);

        choice = kb.Open();
        if (choice < 0)
            goto ask;

        bool all = choice == 0;

        if (all) {
            size_t count = 0;
            for (Title& title : Cheat::titles)
                count += Game::unlock(Offset::LIB_TITLES, title.id);

            MessageBoxPlus::wrap(String::success,
            Color::Yellow << std::to_string(count) << Color::Silver << " title" << String::plural(count) << " ha" << (count == 1 ? "s": "ve") << String::unlockedTitles);
            goto ask;
        }
        else {
            Title& title = Cheat::titles[choice - 1];
            if (!Game::isUnlocked(Offset::LIB_TITLES, title.id)) {
                Game::unlock(Offset::LIB_TITLES, title.id);

                MessageBoxPlus::wrap(String::success,
                Color::Silver << "Title " << String::bold(title.name) << Color::Silver << " has been unlocked!");
            }
            else
                MessageBoxPlus::wrap(String::error, Color::Silver << "You already unlocked " << String::bold(title.name) << Color::Silver << "!");

            goto askTitle;
        }
    }
}
    
void Cheat::grandEstarkEventFix(MenuEntry* entry) {
    u32 addrSave, addrField1, addrField2;
    if (!PTR::get(PTR::Type::SAVE, addrSave))
        return;

    const u8 flag_5ItemsHoganTalked = 1 << 5,
             flag_inspectedMachine1 = 1 << 7,
             flag_inspectedMachine2 = 1 << 3;

    u8 field1, field2;
    addrField1 = addrSave + Offset::GRAND_ESTARK_FIX_BITFIELD1;
    addrField2 = addrSave + Offset::GRAND_ESTARK_FIX_BITFIELD2;
    Process::Read8(addrField1, field1);
    Process::Read8(addrField2, field2);

    // check if part of this event is already done
    if ((field1 & flag_inspectedMachine1) && (field2 & flag_inspectedMachine2)) {
        MessageBoxPlus::wrap(String::error, String::estarkAlreadyDone);
        return;
    }

    // check if player has all 5 needed items
    for (size_t i = 0; i < Macro::GRAND_ESTARK_ITEM_COUNT; i++) {
        u16 itemId;
        Process::Read16(addrSave + Offset::GRAND_ESTARK_INVENTORY_ITEMS + i * sizeof(itemId), itemId);
        if (itemId == 0) {
            MessageBoxPlus::wrap(String::error, String::estarkNot5Items);
            return;
        }
    }

    // has talked to Hogan about the 5 items?
    if (!(field1 & flag_5ItemsHoganTalked)) {
        MessageBoxPlus::wrap(String::error, String::estarkRoryBellows);
        return;
    }

    // set machine to inspected
    field1 |= flag_inspectedMachine1;
    field2 |= flag_inspectedMachine2;
    Process::Write8(addrField1, field1);
    Process::Write8(addrField2, field2);

    // remove the 5 items
    for (size_t i = 0; i < Macro::GRAND_ESTARK_ITEM_COUNT; i++)
        Process::Write16(addrSave + Offset::GRAND_ESTARK_INVENTORY_ITEMS + i * sizeof(u16), 0);

    MessageBoxPlus::wrap(String::success, String::estarkFixed);
}

void Cheat::wiFiCoins(MenuEntry* entry) {
    u32 addr;
    if (!PTR::get(PTR::Type::SAVE, addr))
        return;

    static std::array<std::string, 4> coinNames = {String::bronzeCoin, String::silverCoin, String::goldCoin, String::platinumCoin};

    Keyboard kb(CenterAlign(String::coinAdd));
    kb.Populate(Cheat::coinIcons);
    kb.IsHexadecimal(false);
    kb.SetMaxLength(3);

    int choice = kb.Open();
    if (choice < 0)
        return;

    u16 coins;
    addr = addr + Offset::WIFI_COINS + choice * sizeof(coins);
    Process::Read16(addr, coins);

    kb.GetMessage() = CenterAlign("\nEnter coin amount for\n" << String::bold(coinNames[choice]));
    if (kb.Open(coins, coins) >= 0)
        Process::Write16(addr, std::min(coins, static_cast<u16>(Macro::MAX_WIFI_COINS)));

    Cheat::wiFiCoins(entry);
}

void Cheat::j3Transfer(Item* item) {
    u32 addr, flags;
    PTR::get(PTR::Type::SAVE, addr);
    addr += Offset::J3_TRANSFERED_BITS;

    Process::Read32(addr, flags);
    flags |= (1 << 23); // has transfered monster from J3?
    flags |= (1 << 24); // has claimed ticket (and starter) from Wood Park machine?
    Process::Write32(addr, flags);
}

void Cheat::fluffyScruffyBooks(Item* item) {
    static std::unordered_map<u16, u32> statueBits =  {
        {0x3E3, 1 << 14}, // Fluffy 1
        {0x3E4, 1 << 15}, // Fluffy 2
        {0x3E5, 1 << 16}, // Scruffy 1
        {0x3E6, 1 << 17}, // Scruffy 2
    };

    if (!item || !statueBits.contains(item->id))
        return;

    u32 addr, flags;
    PTR::get(PTR::Type::SAVE, addr);
    addr += Offset::WIFI_STATUES_BITS;

    u32 statueBit = statueBits[item->id];
    Process::Read32(addr, flags);
    flags |= statueBit;
    Process::Write32(addr, flags);
}

void Cheat::wiFiCoinPatch() {
    Process::Write32(ARM::WIFI_COIN_ANTICHEAT1, 0xEA000040);
    Process::Write32(ARM::WIFI_COIN_ANTICHEAT2, Macro::BX_LR);
}

void Cheat::sirloinPatch() {
    Process::Write32(ARM::SIRLOIN_ANTICHEAT1, Macro::BX_LR);
    Process::Write32(ARM::SIRLOIN_ANTICHEAT2, Macro::NOP);
    Process::Write32(ARM::SIRLOIN_ANTICHEAT3, Macro::NOP);
}

std::string Cheat::invalidMonstersWarning(bool single) {
    std::string monsterMonsters = single ? "monster" : "monsters",
                itThem = single ? "it" : "them";
    return (Color::Orange << "WARNING!" << Color::Silver << " The generated " << monsterMonsters << (single ? " is an " : "are ") << Color::Red << "invalid monster" << (single ? "" : "s") << Color::Silver << ". If you want " << itThem << " to be valid, you need to buy " << itThem << " at the Wi-Fi Square shop.\nCheck the cheat notes for more info about invalid monsters.");
}

bool KBCallback::parseEvent(KeyboardEvent& event, int& choice, bool& all) {
    if (event.type != KeyboardEvent::EventType::SelectionChanged)
        return false;

    // event is triggered if scrolling with touch screen (glitch?)
    if (event.selectedIndex == -1)
        return false;

    choice = event.selectedIndex;
    all = choice == 0;

    return true;
}

template<typename T>
void KBCallback::init(Keyboard& kb, KeyboardEvent& event, std::vector<T>& content, Offset unlockType, const std::string& baseMessage) {
    int choice;
    bool all;
    
    if (!KBCallback::parseEvent(event, choice, all))
        return;

    std::string base = CenterAlign(baseMessage);

    if (all) {
        kb.GetMessage() = base;
        return;
    }

    T& thing = content[choice - 1];
    bool isUnlocked = Game::isUnlocked(unlockType, thing.id);

    std::string how = "\n\nHow to unlock " << String::bold(thing.name) << Color::Silver << ":\n" << thing.unlockMethod,
                unlocked = Color::White << "\n\nUnlocked? " << (isUnlocked ? String::yes : String::no);

    if constexpr (std::is_same_v<T, Monster>) {
        bool inStorage = Storage::isInStorage(thing);
        unlocked += (Color::White << "\nIn storage? " << (inStorage ? String::yes : String::no));
    }

    kb.GetMessage() = base << how << unlocked;
}

void KBCallback::unobtainableContent_monsters_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::monsters, Offset::LIB_MONSTERS, String::chooseMonsterUnlock);
}

void KBCallback::unobtainableContent_skill_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::skills, Offset::LIB_SKILLS, String::chooseSkillUnlock);
}

void KBCallback::unobtainableContent_items_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::items, Offset::LIB_ITEMS, String::chooseItemUnlock);
}

void KBCallback::unobtainableContent_titles_cb(Keyboard& kb, KeyboardEvent& event) {
    KBCallback::init(kb, event, Cheat::titles, Offset::LIB_TITLES, String::chooseTitleUnlock);
}
