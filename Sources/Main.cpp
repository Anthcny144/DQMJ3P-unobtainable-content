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

    void loop() {
        if (!settings.has("opened")) {
            u32 pos = static_cast<float>(Macro::SCREEN_BOTTOM_WIDTH) / 2 - OSD::GetTextWidth(false, String::pressSelect) / 2;
            top.Draw(String::pressSelect, pos + 1, 211, Color::Black, Color(0, 0, 0, 0));
            top.Draw(String::pressSelect, pos, 210, Color::White, Color(0, 0, 0, 0));
        }
    }

    bool onOpen() {
        if (!settings.has("opened")) {
            settings.set("opened", "1");
            settings.save();
        }

        return true;
    }

    int main() {
        if (!GameTitle::check())
            return 0;

        while (!Game::isLoaded())
            Sleep(Seconds(0.2));

        PluginMenu *_3gx = new PluginMenu(String::_3gxTitle, 3, 1, 1, String::_3gxNotes);

        settings.open(SETTINGS_BIN);

        _3gx->Append(new MenuEntry(String::cheat_unobtainableContent, nullptr, Cheat::unobtainableContent, String::notes_unobtainableContent));
        _3gx->Append(new MenuEntry(String::cheat_grandEstarkEventFix, nullptr, Cheat::grandEstarkEventFix, String::notes_grandEstarkEventFix));
        _3gx->Append(new MenuEntry(String::cheat_wiFiCoins, nullptr, Cheat::wiFiCoins, String::notes_wiFiCoins));

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
