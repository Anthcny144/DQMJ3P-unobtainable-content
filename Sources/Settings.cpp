#include <CTRPluginFramework.hpp>
#include "CTRPFExtension.hpp"
#include "Settings.hpp"
#include "String.hpp"
#include <vector>
#include <utility>
using namespace CTRPluginFramework;

Settings::Settings() {}

Settings::Settings(const std::string& path) {
    open(path);
}

void Settings::initData(File& file) {
    u32 size = file.GetSize();
    std::vector<u8> readBuffer(size);
    std::vector<std::string> argsBuffer;
    file.Read(readBuffer.data(), size);

    std::string word = "";

    for (u8 byte : readBuffer) {
        if (byte == '\0') {
            if (!word.empty()) {
                argsBuffer.push_back(std::move(word));
                word.clear();
            }
        }

        else
            word += static_cast<char>(byte);
    }

    // invalid arg count
    if (argsBuffer.size() % 2 != 0) {
        MessageBoxPlus::wrap(String::error, Color::Silver << "Odd arg count in settings file (" << Color::Yellow << file.GetFullName() << Color::Silver << ")");
        return;
    }

    for (int i = 0; i < argsBuffer.size() / 2; i++) {
        std::string key = std::move(argsBuffer[i]);
        std::string val = std::move(argsBuffer[i + 1]);

        data[std::move(key)] = std::move(val);
    }
}

bool Settings::open(const std::string& path) {
    this->path = path;

    File file;
    bool exists = File::Exists(path) == 1;

    if (exists) {
        // check for path validity
        if (File::Open(file, path, File::READ) < 0)
            return false;

        isValid = true;
        initData(file);
        file.Close();
    }

    return exists;
}

bool Settings::has(const std::string& key) {
    return data.contains(key);
}

std::string Settings::get(const std::string& key) {
    return has(key) ? data[key] : "";
}

void Settings::set(const std::string& key, const std::string& value, bool save) {
    data[key] = value;

    if (save)
        this->save();
}

void Settings::save() {
    File file;
    if (File::Open(file, path, File::CREATE | File::WRITE) < 0)
        return;

    for (const auto& pair : data) {
        u32 keySize = static_cast<u32>(pair.first.size() + 1);
        u32 valSize = static_cast<u32>(pair.second.size() + 1);

        file.Write(pair.first.data(), keySize);
        file.Write(pair.second.data(), valSize);
    }

    file.Flush();
    file.Close();
}