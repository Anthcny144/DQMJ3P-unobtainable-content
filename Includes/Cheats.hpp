#pragma once
#include <CTRPluginFramework.hpp>
#include "Content.hpp"
#include "Game.hpp"
#include "String.hpp"
#include <sstream>
using namespace CTRPluginFramework;

struct KBChoice {
    int idx;
    bool exit;
    std::string saveKbMessage;
    KBChoice() {
        idx = 0;
        exit = true;
        saveKbMessage = "";
    };

    KBChoice(int idx, bool exit) :
    idx(idx), exit(exit) {};
};

struct KBMessageParts {
    const std::string title, unlocked, storage;
    const std::vector<std::string> how;

    KBMessageParts() {};
    KBMessageParts(const std::string& title, const std::vector<std::string>& how, const std::string& unlocked, const std::string& storage) :
    title(title), how(how), unlocked(unlocked), storage(storage) {};

    std::string concat() {
        std::string text = "";

        text += CenterAlign(title, 30);
        text += "\n\n";
        
        for (const std::string& howPart : how)
            text += (howPart + "\n");
        
        text += "\n";
        text += unlocked;

        text += "\n";
        text += storage;

        return text;
    }
};

struct Cheat {
    static void init();

    static void unobtainableContent(MenuEntry* entry);

    static KBChoice giveMonster(Keyboard& kb, int selectedIdx, const std::string& message);
    static bool monstersMenu();
    static KBChoice giveSkill(Keyboard& kb, int selectedIdx, const std::string& message);
    static KBChoice giveItem(Keyboard& kb, int selectedIdx, const std::string& message);
    static KBChoice giveTitle(Keyboard& kb, int selectedIdx, const std::string& message);
    static void j3Transfer(Item* item = nullptr);
    static void cottneyRottneyAlbums(Item* item);

    static bool isMonsterInStorage(u16 monsterId);
    
    static std::vector<CustomIcon> monsterIcons, coinIcons;
    static std::vector<Monster> monsters;
    static std::vector<Skill> skills;
    static std::vector<Item> items;
    static std::vector<Title> titles;
    static std::vector<std::string> skillNames, itemNames, titleNames;

    private:
    static void _writeMonsterChecksums();
    static Addr _monstersCanBuyAddr;
    
    friend class KBCallback;
};

struct KBCallback {
    static bool parseEvent(KeyboardEvent& event, int& choice, bool& all);

    template<typename T>
    static void init(Keyboard& kb, KeyboardEvent& event, std::vector<T>& content, u32 unlockOffset, const std::string& baseMessage) {
        int choice;
        bool all;

        if (!KBCallback::parseEvent(event, choice, all))
            return;

        kb.GetMessage() = KBCallback::getMessageParts(content, unlockOffset, baseMessage, choice, all).concat();
    }

    template<typename T>
    static KBMessageParts getMessageParts(std::vector<T>& content, u32 unlockOffset, const std::string& baseMessage, int choice, bool all) {
        std::string title, unlocked, storage;

        title = baseMessage;

        if (all)
            return KBMessageParts(title, {}, "", "");
        
        T& thing = content[choice - 1];
        bool isUnlocked = Game::isUnlocked(unlockOffset, thing.id);

        std::vector<std::string> how;
        std::string temp;
        std::stringstream stream(thing.unlockMethod);

        how.push_back("How to unlock " << String::bold(thing.colorName) << Color::Silver << ":");
        while (std::getline(stream, temp, '\n'))
            how.push_back(Color::Silver << temp);

        unlocked = Color::White << "Unlocked? " << (isUnlocked ? String::yes : String::no);
        
        if constexpr (std::is_same_v<T, Monster>) {
            bool inStorage = Cheat::isMonsterInStorage(thing.id);
            storage = Color::White << "In storage? " << (inStorage ? String::yes : String::no);
        }

        return KBMessageParts(title, how, unlocked, storage);
    }

    static void unobtainableContent_monsters_cb(Keyboard& kb, KeyboardEvent& event);
    static void unobtainableContent_skill_cb(Keyboard& kb, KeyboardEvent& event);
    static void unobtainableContent_items_cb(Keyboard& kb, KeyboardEvent& event);
    static void unobtainableContent_titles_cb(Keyboard& kb, KeyboardEvent& event);
};
