#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "CTRPFExtension.hpp"
#include "Game.hpp"
#include "GameTitle.hpp"
#include "Macro.hpp"
#include "Migrate.hpp"
#include "Settings.hpp"
#include "Text.hpp"

namespace CTRPluginFramework {
    const std::array<u32, 3> pluginVersion = {5, 0, 0};

    void PatchProcess(FwkSettings& fwk) {
        fwk.WaitTimeToBoot = Seconds(15);
        fwk.Notifications.LifeSpan = Seconds(4);
    }

    void loop() {
        static bool lock = false;

        if (!Settings::has("opened")) {
            if (!lock)
                Process::Pause();
            
            static const Screen& top = OSD::GetTopScreen();

            if (!lock) {
                Process::Play();
                lock = true;
            }

            std::string text = Text::get("PLUGIN_PRESS_SELECT");
            u32 pos = static_cast<float>(Macro::SCREEN_TOP_WIDTH) / 2 - OSD::GetTextWidth(true, text) / 2;
            top.DrawSysfont(text, pos + 1, 211, Color::Black);
            top.DrawSysfont(text, pos, 210, Color::White);
        }
    }

    bool onOpen() {
        if (!Settings::has("opened"))
            Settings::set("opened", "1", true);

        return true;
    }

    bool initTexts(std::vector<CustomIcon>& flagIcons, bool forceSystemLanguage) {
        LanguageId language;

        // Get language from system
        if (forceSystemLanguage) {
            LanguageId sysLang = Text::getSystemLanguage();
            language = sysLang;
            Text::language = language;
        }
        // Ask for language
        else if (!Settings::has("language")) {
            LanguageId sysLang = Text::getSystemLanguage();
            Text::language = sysLang;
            
            Keyboard kb(CenterAlign(Text::getChooseLanguageText(sysLang)));
            kb.CanAbort(false);
            kb.Populate(flagIcons);

            // Exit if user exited keyboard
            int choice = kb.Open();
            if (choice < 0)
                return false;

            language = Text::getLanguageByIndex(choice);
            Text::language = language;
            Settings::set("language", std::to_string(choice), true);
        }
        else {
            language = Text::getLanguageFromSettings();
            Text::language = language;
        }

        // Load texts
        return Text::tryLoad(language);
    }

    bool loadIconsPixels(std::vector<CustomIcon::Pixel>& pixels) {
        File iconsFile;
        if (File::Open(iconsFile, Macro::PLUGIN_FOLDER + "/icons.bin", File::READ) != File::SUCCESS) {
            return false;
        }

        u64 fileSize = iconsFile.GetSize();
        pixels.resize(fileSize / sizeof(CustomIcon::Pixel));
        return iconsFile.Read(pixels.data(), fileSize) == File::SUCCESS;
    }

    int exit() {
        ProcessPlus::Exit();
        return 1;
    }

    int main() {
        // Check game
        int gameOk = GameTitle::init();

        if (gameOk == 0) {
            // Migrate to 5.0.0
            Migrate::to__5_0_0();
    
            // Load settings
            Settings::init(Macro::PLUGIN_FOLDER + "/settings.bin");
            std::string pluginVersionStr = std::to_string(pluginVersion[0]) + "." + std::to_string(pluginVersion[1]) + "." + std::to_string(pluginVersion[2]);
            Settings::set("3gx_version", pluginVersionStr, true);
        }

        // Load icons file
        std::vector<CustomIcon::Pixel> iconsPixels;
        bool iconsOk = loadIconsPixels(iconsPixels);

        // Load texts
        std::vector<CustomIcon> flagIcons = Text::loadLanguageIcons(iconsPixels);
        if (!initTexts(flagIcons, gameOk != 0 || !iconsOk))
            exit();

        // Show wrong game error
        if (gameOk != 0) {
            if (gameOk == 1)
                MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_ERROR", Color::Red)), Text::get("PLUGIN_WRONG_GAME", Color::White, ColorPlus::None, {Text::get("PLUGIN_NAME") << Color::Silver, Text::getUniversal("DQMJ3") << Color::Silver, Text::getUniversal("DQMJ3P") << Color::Silver}));
            else if (gameOk == 2)
                MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_WARNING", Color::Orange)), Text::get("PLUGIN_UNUSABLE_VERSION", Color::Silver, ColorPlus::None, {Text::getUniversal("DISCORD_URL")}));

            exit();
        }

        // Show icon loading error
        if (!iconsOk) {
            MessageBoxPlus::wrap(Text::bold(Text::get("MSGBOX_ERROR", Color::Red)), Text::get("ICONS_BIN_NOT_LOADED", Color::Silver));
            exit();
        }

        // Init cheats-related stuff
        Cheat::init(iconsPixels, flagIcons);

        std::string gameName = Text::getUniversal(GameTitle::isJ3P() ? "DQMJ3P" : "DQMJ3"),
                    title = Text::bold(gameName + " " + Text::get("PLUGIN_NAME", Color::White));

        PluginMenu* _3gx = new PluginMenu(title, pluginVersion[0], pluginVersion[1], pluginVersion[2], Text::get("PLUGIN_DESC", Color::Silver, ColorPlus::None, {Color::Silver, Text::getUniversal("PLUGIN_AUTHOR"), Text::getUniversal("DISCORD_URL")}));

        _3gx->Append(new MenuEntry(Text::bold(Text::gradient(Text::get("CHEAT_UNOBTAINABLE_CONTENT_NAME"), Color::Red, Color::Blue)), nullptr, Cheat::unobtainableContent, Text::get("CHEAT_UNOBTAINABLE_CONTENT_DESC", Color::Silver, Color::Silver)));
        _3gx->Append(new MenuEntry(Text::bold(Text::gradient(Text::get("CHEAT_WIFI_COINS_NAME"), Gradient::Gold.first, Gradient::Gold.second, Gradient::Gold.power)), nullptr, Cheat::wiFiCoins, Text::get("CHEAT_WIFI_COINS_DESC", Color::Silver)));
        _3gx->Append(new MenuEntry(Text::bold(Color::Silver << Text::get("CHEAT_SWITCH_LANGUAGE_NAME")), nullptr, Cheat::switchLanguage, Text::get("CHEAT_SWITCH_LANGUAGE_DESC", Color::Silver)));

        _3gx->SynchronizeWithFrame(true);
        _3gx->ShowWelcomeMessage(false);

        _3gx->Callback(loop);
        _3gx->OnOpening = onOpen;

        OSD::Notify(Text::gradient(Text::get("PLUGIN_READY"), Color::Purple, GameTitle::isJ3P() ? Color::Yellow : Color::Red, 0.75f));

        _3gx->Run();
        delete _3gx;
        return 0; 
    }
}

/*

BUG
- crash if monster cheat enabled while hovering a monster family without any monster at the WiFi Square
- txt parsing: \n{0} freezes if {0} if arg is a color (jp.txt PLUGIN_DESC arg 0 is 1 char late because of this issue)

TODO
- set monsters coin price to 0 (can remove the not enough coins hook)
- make a clean migration function that takes old version (from settings, or defaults to < 5.0.0 if none) and new version
- update system that fetches the latest release from Github (version & patchnotes)

########################################################################################################################

monster for free at wifi square (wip)

coin price: 7C7AA4 

hook hide coin price:
44766C bl hook1

hook1
mov r2, #0 // set coin price to 0
mov r3, #0 // set coin price to 0
str r2, [r1]
str r3, [r1, #4]
bx lr

*/