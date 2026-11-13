#pragma once
#include <CTRPluginFramework.hpp>
using namespace CTRPluginFramework;

struct Macro {
    enum {
        MAX_MONSTER_STORAGE_SPACE = 500,
        MAX_ITEM_COUNT = 999,
        CHECKSUM_SLOTS_J3 = 20,
        CHECKSUM_SLOTS_J3P = 40,
    
        FREE_RAM_SPACE_ADDR = 0x1E81000,
        FREE_RAM_SPACE_SIZE = 0x1000,
    
        SCREEN_TOP_WIDTH = 400,
        SCREEN_BOT_WIDTH = 320,
        SCREEN_HEIGHT = 240,
    
        SCREEN_TOP_MENU_X = 30,
        SCREEN_TOP_MENU_Y = 20,
        SCREEN_TOP_MENU_WIDTH = 340,
        SCREEN_BOT_MENU_X = 20,
        SCREEN_BOT_MENU_Y = 20,
        SCREEN_BOT_MENU_WIDTH = 280,
        SCREEN_MENU_HEIGHT = 200
    };

    static inline const std::string PLUGIN_FOLDER = "UnobtainableContentFiles";
};