#pragma once
#include <CTRPluginFramework.hpp>
#include "Content.hpp"
#include "CTRPFExtension.hpp"
#include <cmath>
#include <unordered_map>
using namespace CTRPluginFramework;

struct TextLoadRes {
    bool ok;
    int errorIdx;
    int lineIdx;

    TextLoadRes(bool ok);
    TextLoadRes(bool ok, int errorIdx);
    TextLoadRes(bool ok, int errorIdx, int lineIdx);
};

struct Text {
    static LanguageId language;
    
    static std::vector<CustomIcon> loadLanguageIcons(std::vector<CustomIcon::Pixel>& iconsPixels);
    static LanguageId getSystemLanguage();
    static LanguageId getLanguageFromSettings();
    static LanguageId getLanguageByIndex(int index);

    static bool tryLoad(LanguageId language);
    static TextLoadRes load(LanguageId language);
    static std::string getChooseLanguageText(LanguageId language);
    static std::string getError(LanguageId language, TextLoadRes loadRes);
    static std::string getReinstallMessage(LanguageId language);

    static std::string get(const std::string& key, const Color inColor) {
        return Text::_build(key, {}, inColor, ColorPlus::None);
    }

    static std::string get(const std::string& key, std::initializer_list<std::string> args = {}) {
        return Text::_build(key, args, ColorPlus::None, ColorPlus::None);
    }

    static std::string get(const std::string& key,
                        const Color inColor,
                        const Color outColor,
                        std::initializer_list<std::string> args = {}) {
        return Text::_build(key, args, inColor, outColor);
    }
    
    static std::string getUniversal(const std::string& key);

    static void loadContentTexts(std::vector<Skill>& skills, std::vector<std::string>& skillNames,
                                 std::vector<Item>& items, std::vector<std::string>& itemNames,
                                 std::vector<Title>& titles, std::vector<std::string>& titleNames);

    static inline std::string gradient(const std::string& text, Color from, Color to, float power = 0.5f) {
        std::vector<std::string> glyphs;
        for (size_t i = 0; i < text.size(); ) {
            size_t len = std::min(Text::_utf8SeqLen(static_cast<unsigned char>(text[i])), text.size() - i);
            glyphs.emplace_back(text.substr(i, len));
            i += len;
        }

        const size_t n = glyphs.size();
        const float exponent = std::log(0.5f) / std::log(power);
        std::string newText;

        for (size_t i = 0; i < n; i++) {
            float ratio = (n == 1) ? 0.0f : float(i) / (n - 1);
            float weightRatio = std::pow(ratio, exponent);

            u8 r = from.r + (to.r - from.r) * weightRatio,
            g = from.g + (to.g - from.g) * weightRatio,
            b = from.b + (to.b - from.b) * weightRatio;

            newText += Color(r, g, b, 0xFF);
            newText += glyphs[i];
        }

        return newText;
    }

    static inline std::string bold(const std::string& text) {
        return ToggleDrawMode(Render::BOLD) << text << ToggleDrawMode(Render::BOLD);
    }

    static inline std::string underline(const std::string& text) {
        return ToggleDrawMode(Render::UNDERLINE) << text << ToggleDrawMode(Render::UNDERLINE);
    }

    static inline int centerSysFont(const std::string& text, int startX, int width) {
        const int fontWidth = OSD::GetTextWidth(true, text);
        return (int)(width / 2 - fontWidth / 2 + startX);
    }

    private:
    static bool _parseLine(std::string line);
    static std::string _unescape(const std::string& text);
    static int _hexVal(char c);
    static void _appendUtf8(std::string& out, u32 code);
    static std::string _format(const std::string& tpl, const std::vector<std::string>& args);
    static std::string _build(const std::string& key,
                              std::initializer_list<std::string> args,
                              Color inColor,
                              Color outColor);
    static size_t _utf8SeqLen(unsigned char c);

    static std::unordered_map<std::string, std::string> _texts, _universal;
    static std::unordered_map<LanguageId, std::string> _languageCodes, _chooseLanguage, _reinstallMessages;
    static std::unordered_map<LanguageId, std::array<std::string, 4>> _loadErrors;
};