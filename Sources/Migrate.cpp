#include <CTRPluginFramework.hpp>
#include "Macro.hpp"
#include "Migrate.hpp"
using namespace CTRPluginFramework;

/*
For versions prior to 5.0.0:
- MOVED: Icons/*.bin -> UnobtainableContentFiles/icons.bin (in one single file)
- MOVED: settings.bin -> UnobtainableContentFiles/settings.bin
- ADDED: UnobtainableContentFiles/Texts/*.txt
*/
void Migrate::to__5_0_0() {
    // Remove old Icons folder
    if (Directory::IsExists("Icons"))
        Directory::Remove("Icons");

    // Move settings.bin
    if (File::Exists("settings.bin")) {
        File oldSettings, newSettings;
        if (File::Open(oldSettings, "settings.bin", File::READ) != File::SUCCESS || File::Open(newSettings, Macro::PLUGIN_FOLDER + "/settings.bin", File::WRITE | File::CREATE) != File::SUCCESS)
            return;

        std::string content(oldSettings.GetSize(), '\0');
        if (oldSettings.Read(content.data(), oldSettings.GetSize()) != File::SUCCESS)
            return;

        if (newSettings.Write(content.data(), content.length()) != File::SUCCESS)
            return;

        oldSettings.Close();
        newSettings.Flush();
        newSettings.Close();

        File::Remove("settings.bin");
    }
}