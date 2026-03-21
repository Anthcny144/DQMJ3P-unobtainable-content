#include <CTRPluginFramework.hpp>
#include "Cheats.hpp"
#include "CTRPFExtension.hpp"
#include "Game.hpp"
#include "GameTitle.hpp"
#include "Macro.hpp"
#include "Settings.hpp"
#include "String.hpp"

namespace CTRPluginFramework {
    Settings settings = Settings();
    const Screen& top = OSD::GetTopScreen();

    void PatchProcess(FwkSettings& fwk) {
        fwk.WaitTimeToBoot = Seconds(10);
        fwk.Notifications.LifeSpan = Seconds(8);
    }

    void debugAddr() {
        u32 drawY = 0;
        for (int i = 0; i <= static_cast<int>(ARM::AddrType::UNBUYABLE_MONSTERS); i++) {
            ARM::AddrType type = static_cast<ARM::AddrType>(i);
            Addr addr = ARM::getAddr(type);
            std::string text;

            if (addr != ARM::UNDEF) {
                u32 value;
                Process::Read32(addr, value);
                text = Utils::Format("%08X", addr ) + " " + Utils::Format("%08X", value);
            }
            else
                text = "PRO ONLY ADDRESS";

            drawY = top.Draw(text, 0, drawY);
        }
    }

    void loop() {
        if (!settings.has("opened")) {
            u32 pos = static_cast<float>(Macro::SCREEN_BOT_WIDTH) / 2 - OSD::GetTextWidth(false, String::pressSelect) / 2;
            top.Draw(String::pressSelect, pos + 1, 211, Color::Black, Color(0, 0, 0, 0));
            top.Draw(String::pressSelect, pos, 210, Color::White, Color(0, 0, 0, 0));
        }
    }

    bool onOpen() {
        if (!settings.has("opened"))
            settings.set("opened", "1", true);

        return true;
    }

    int main() {
        if (!GameTitle::init())
            return 0;

        while (!Game::isLoaded())
            Sleep(Seconds(0.2));

        Sleep(Seconds(4));

        std::string gameName = GameTitle::isJ3P() ? String::dqmj3p : String::dqmj3,
                    title = String::bold(gameName + " " + String::unobtainableContent);

        PluginMenu *_3gx = new PluginMenu(title, 4, 0, 0, String::pluginNotes);

        settings.open(SETTINGS_BIN);

        _3gx->Append(new MenuEntry(String::cheat_unobtainableContent, nullptr, Cheat::unobtainableContent, String::notes_unobtainableContent));

        _3gx->SynchronizeWithFrame(true);
        _3gx->ShowWelcomeMessage(false);

        _3gx->Callback(loop);
        _3gx->OnOpening = onOpen;

        Cheat::init();
        OSD::Notify(String::welcome);

        _3gx->Run();
        delete _3gx;
        return 0; 
    }
}

/*

TODO
- enable hook by default for buying monsters if not enough coins (to prevent emu glitch)
- init text on plugin ready + make translations for japanese, english and french (and italian? https://github.com/Lurpigi/DQMJ3P-IT-FanTranslation/)
*/