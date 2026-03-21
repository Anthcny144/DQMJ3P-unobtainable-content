#pragma once
#include <CTRPluginFramework.hpp>
#include "CTRPFExtension.hpp"
#include "GameTitle.hpp"
#include "Memory.hpp"
#include <array>
#include <functional>
#include <vector>
using namespace CTRPluginFramework;

struct Content {
    const std::string name, unlockMethod;
    std::string colorName;
    const u16 id;
    VersionBits version;

    Content(const std::string& name,
            const u16 id,
            Gradient gradient,
            const std::string& unlockMethod,
            VersionBits version
    );

    virtual ~Content() = default;

    template<typename T>
    static std::vector<T> get(std::vector<T>& contentList) {
        std::vector<T> filteredList;

        for (T& thing : contentList)
            if (static_cast<u32>(thing.version) & static_cast<u32>(GameTitle::versionBits))
                filteredList.emplace_back(thing);

        return filteredList;
    }
};

struct Monster : public Content {
    enum Size {
        SMALL = 0x46,
        NORMAL = 0x47,
        MEGA = 0x48,
        GIGA = 0x49,
        ULTRA = 0x4A
    };

    const std::string iconName;
    const u32 checksum;

    Monster(const std::string& name,
            const std::string& iconName,
            Gradient gradient,
            u16 id,
            const std::string& unlockMethod,
            u32 checksum,
            VersionBits version
    );

    static std::vector<Monster>& getList();
};

struct Skill : public Content {
    const std::string bookName;
    const u16 bookId;

    Skill(const std::string& name,
          Gradient gradient,
          u16 id,
          u16 bookId,
          const std::string& bookName,
          const std::string& unlockMethod,
          VersionBits version = VersionBits::ALL
    );
    
    static std::vector<Skill>& getList();
};

struct Item : public Content {
    const bool unlockableOnce;
    const std::function<void(Item*)> onUnlock;

    Item(const std::string& name,
         Gradient gradient,
         u16 id,
         bool unlockableOnce,
         const std::string& unlockMethod,
         std::function<void(Item*)> onUnlock = nullptr,
         VersionBits version = VersionBits::ALL
    );
    
    static std::vector<Item>& getList();
};

struct Title : public Content {
    Title(const std::string& name,
          u16 id,
          Gradient gradient,
          const std::string& unlockMethod,
          VersionBits version = VersionBits::ALL
    );
    
    static std::vector<Title>& getList();
};