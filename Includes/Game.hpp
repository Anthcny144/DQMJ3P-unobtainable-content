#pragma once
#include <CTRPluginFramework.hpp>
#include "Memory.hpp"
using namespace CTRPluginFramework;

struct Game {
    static bool isLoaded();
    static bool unlock(u32 offset, u16 id);
    static bool isUnlocked(u32 offset, u16 id);
    static void increaseItemCount(u16 itemId);
};