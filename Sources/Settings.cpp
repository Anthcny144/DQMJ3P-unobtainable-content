#include <CTRPluginFramework.hpp>
#include "CTRPFExtension.hpp"
#include "Settings.hpp"
#include <vector>
#include <utility>
using namespace CTRPluginFramework;

std::string Settings::_path;
std::unordered_map<std::string, std::string> Settings::_data;

void Settings::_initData(File& file) {
    LineReader reader = LineReader(file);
    std::string line;

    while (reader(line)) {
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos)
            continue; // skip invalid lines

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);
        Settings::_data[key] = value;
    }
}

void Settings::init(const std::string& path) {
    Settings::_path = path;

    File file;
    bool exists = File::Exists(Settings::_path) == 1;

    if (exists) {
        // Check for path validity
        if (File::Open(file, Settings::_path, File::READ) < 0)
            return;

        Settings::_initData(file);
        file.Close();
    }
}

bool Settings::has(const std::string& key) {
    return Settings::_data.contains(key);
}

std::string Settings::get(const std::string& key) {
    return Settings::has(key) ? Settings::_data[key] : "";
}

void Settings::set(const std::string& key, const std::string& value, bool save) {
    Settings::_data[key] = value;

    if (save)
        Settings::save();
}

void Settings::save() {
    File file;
    if (File::Open(file, Settings::_path, File::CREATE | File::WRITE) < 0)
        return;

    for (const auto& pair : _data) {
        std::string line = pair.first + '=' + pair.second + '\n';
        file.Write(line.data(), line.length());
    }

    file.Flush();
    file.Close();
}