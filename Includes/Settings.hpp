#pragma once
#include <CTRPluginFramework.hpp>
#include <unordered_map>
using namespace CTRPluginFramework;

struct Settings {
    static void init(const std::string& path);
    static bool has(const std::string& key);
    static std::string get(const std::string& key);
    static void set(const std::string& key, const std::string& value, bool save);
    static void save();

    private:
    static void _initData(File& file);

    static std::string _path;
    static std::unordered_map<std::string, std::string> _data;
};