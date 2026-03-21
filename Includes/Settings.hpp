#pragma once
#include <CTRPluginFramework.hpp>
#include <unordered_map>
using namespace CTRPluginFramework;

class Settings {
    std::string path;
    std::unordered_map<std::string, std::string> data;

    void initData(File& file);

    public:
    Settings();
    Settings(const std::string& path);
    bool isValid = false;

    bool open(const std::string& path);
    bool has(const std::string& key);
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value, bool save);
    void save();
};