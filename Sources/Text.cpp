#include <CTRPluginFramework.hpp>
#include "CTRPFExtension.hpp"
#include "Settings.hpp"
#include "Text.hpp"
using namespace CTRPluginFramework;

LanguageId Text::language;

TextLoadRes::TextLoadRes(bool ok) {
    this->ok = ok;
    this->errorIdx = -1;
    this->lineIdx = -1;
}

TextLoadRes::TextLoadRes(bool ok, int errorIdx) {
    this->ok = ok;
    this->errorIdx = errorIdx;
    this->lineIdx = -1;
}

TextLoadRes::TextLoadRes(bool ok, int errorIdx, int lineIdx) {
    this->ok = ok;
    this->errorIdx = errorIdx;
    this->lineIdx = lineIdx;
}

std::unordered_map<std::string, std::string> Text::_texts, Text::_universal = {
    {"DQMJ3", Text::gradient("DQMJ3", Color::Purple, Color::Red, 0.75f)},
    {"DQMJ3P", Text::gradient("DQMJ3P", Color::Purple, Color::Yellow, 0.75f)},
    {"PLUGIN_AUTHOR", Color(0x4FCAFFFF) << "Anthcny" << Color::Silver},
    {"DISCORD_URL", Text::underline(Color(0x5865F2FF) << "discord.gg/W5yRJpDd5e") << Color::Silver}
};

std::unordered_map<LanguageId, std::array<std::string, 4>> Text::_loadErrors = {
    {LanguageId::English, {"Language not supported.", "Text file not found.", "Could not open the text file.", "An error occured while parsing a line of the text file."}},
    {LanguageId::French, {"Langage non prise en charge.", "Fichier des textes non trouvé.", "Impossible d'ouvrir le fichier des textes.", "Une erreur est survenue lors de la lecture d'une ligne du fichier des textes."}},
    {LanguageId::Italian, {"Lingua non supportata.", "File di testo non trovato.", "Impossibile aprire il file di testo.", "Si è verificato un errore durante l'analisi di una riga del file di testo."}},
    {LanguageId::Spanish, {"Idioma no compatible.", "Archivo de texto no encontrado.", "No se pudo abrir el archivo de texto.", "Se produjo un error al analizar una línea del archivo de texto."}},
    {LanguageId::Japanese, {"サポートされていない言語です。", "テキストファイルが見つかりません。", "テキストファイルを開けませんでした。", "テキストファイルの行を解析中にエラーが発生しました。"}}
};

std::unordered_map<LanguageId, std::string> Text::_languageCodes {
    {LanguageId::English, "en"},
    {LanguageId::French, "fr"},
    {LanguageId::Italian, "it"},
    {LanguageId::Spanish, "es"},
    {LanguageId::Japanese, "jp"}
}, Text::_chooseLanguage = {
    {LanguageId::English, Text::bold("\nSelect the language.")},
    {LanguageId::French, Text::bold("\nSélectionnez la langue.")},
    {LanguageId::Italian, Text::bold("\nSeleziona la lingua.")},
    {LanguageId::Spanish, Text::bold("\nSeleccione el idioma.")},
    {LanguageId::Japanese, Text::bold("\n言語を選択してください。")}
}, Text::_reinstallMessages = {
    {LanguageId::English, "\nPlease reinstall the plugin and its files."},
    {LanguageId::French, "\nVeuillez réinstaller le plugin avec ses fichiers."},
    {LanguageId::Italian, "\nSi prega di reinstallare il plugin e i relativi file."},
    {LanguageId::Spanish, "\nPor favor, reinstale el plugin y sus archivos."},
    {LanguageId::Japanese, "\nプラグインとそのファイルを再インストールしてください。"}
};

std::vector<CustomIcon> Text::loadLanguageIcons(std::vector<CustomIcon::Pixel>& iconsPixels) {
    static std::array<u32, 5> iconIdxs = {
        46, // EN
        47, // FR
        48, // IT
        49, // ES
        50 // JP
    };

    std::vector<CustomIcon> icons;
    for (u32 idx : iconIdxs)
        icons.emplace_back(CustomIconPlus::loadIcon(iconsPixels, idx));

    return icons;
}

LanguageId Text::getSystemLanguage() {
    LanguageId language = System::GetSystemLanguage();

    switch (language) {
        case LanguageId::English:
        case LanguageId::French:
        case LanguageId::Italian:
        case LanguageId::Spanish:
        case LanguageId::Japanese:
            return language;

        default:
            return LanguageId::English;
    }
}

LanguageId Text::getLanguageFromSettings() {
    if (!Settings::has("language"))
        return LanguageId::English;

    int index = stoi(Settings::get("language"));
    return Text::getLanguageByIndex(index);
}

LanguageId Text::getLanguageByIndex(int index) {
    static const std::array<LanguageId, 5> languages = {
        LanguageId::English,
        LanguageId::French,
        LanguageId::Italian,
        LanguageId::Spanish,
        LanguageId::Japanese
    };

    if (index < 0 || index >= languages.size())
        index = 0;

    return languages[index];
}

bool Text::tryLoad(LanguageId language) {
    TextLoadRes loadRes = Text::load(language);

    if (!loadRes.ok) {
        std::string error = Text::getError(language, loadRes);
        MessageBoxPlus::wrap(Color::Red << "/!\\", Color::Silver << error);
    }

    return loadRes.ok;
}

TextLoadRes Text::load(LanguageId language) {
    if (!_languageCodes.contains(language))
        return TextLoadRes(false, 0);

    File file;
    std::string path = Macro::PLUGIN_FOLDER + "/Texts/" + _languageCodes[language] + ".txt";
    bool exists = File::Exists(path) == 1;

    if (exists) {
        int openRes = File::Open(file, path, File::READ);
        if (openRes != File::SUCCESS)
            return TextLoadRes(false, 1);

        LineReader reader = LineReader(file);
        std::string line;
        _texts.clear();

        int lineIdx = 0;
        while (reader(line)) {
            lineIdx++;
            if (!Text::_parseLine(line))
                return TextLoadRes(false, 3, lineIdx);
        }

        return TextLoadRes(true);
    }

    return TextLoadRes(false, 2);
}

std::string Text::getChooseLanguageText(LanguageId language) {
    return _chooseLanguage[language];
}

std::string Text::getError(LanguageId language, TextLoadRes loadRes) {
    std::string error = _loadErrors[language][loadRes.errorIdx];

    if (loadRes.errorIdx > 0)
        error += Text::getReinstallMessage(language);

    if (loadRes.lineIdx != -1)
        error += ("\n" + std::to_string(loadRes.lineIdx));

    return error;
}

std::string Text::getReinstallMessage(LanguageId language) {
    return _reinstallMessages[language];
}

std::string Text::getUniversal(const std::string& key) {
    return _universal.contains(key) ? _universal[key] : ("[" + key + "]");
}

void Text::loadContentTexts(std::vector<Skill>& skills, std::vector<std::string>& skillNames,
                            std::vector<Item>& items, std::vector<std::string>& itemNames,
                            std::vector<Title>& titles, std::vector<std::string>& titleNames)
{
    // All text
    std::string all = Text::bold(Text::get("CHOICE_ALL"));

    // Skills
    skillNames.clear();
    skillNames.push_back(all);
    for (Skill& skill : skills)
        skillNames.push_back(Text::bold(skill.colorName));

    // Items
    itemNames.clear();
    itemNames.push_back(all);
    for (Item& item : items)
        itemNames.push_back(Text::bold(item.colorName));

    // Titles
    titleNames.clear();
    titleNames.push_back(all);
    for (Title& title : titles)
        titleNames.push_back(Text::bold(title.colorName));
}

bool Text::_parseLine(std::string line) {
    // Strip trailing LF / CRLF
    while (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
        line.pop_back();

    // Skip empty lines and comments
    if (line.empty() || line[0] == '#')
        return true;

    // Split on the first '='
    size_t eqPos = line.find('=');
    if (eqPos == std::string::npos)
        return false; // malformed line

    std::string key = line.substr(0, eqPos);
    std::string value = line.substr(eqPos + 1);

    // Empty key
    if (key.empty())
        return true;

    _texts[key] = _unescape(value);
    return true;
}

std::string Text::_unescape(const std::string& text) {
    std::string out;
    out.reserve(text.size());

    for (size_t i = 0; i < text.size(); ++i) {
        // Don't parse if not backslash
        if (text[i] != '\\' || i + 1 >= text.size()) {
            out += text[i];
            continue;
        }

        // Read char after backslash
        char next = text[++i];

        switch (next) {
            case 'n':
                out += '\n';
                break;

            case 't':
                out += '\t';
                break;

            case '\\':
                out += '\\';
                break;

            case 'u': {
                u32 code = 0;
                bool ok = (i + 4 < text.size() + 0) && (i + 4 <= text.size() - 1);
                for (int j = 1; ok && j <= 4; ++j) {
                    int h = Text::_hexVal(text[i + j]);
                    
                    if (h < 0)
                        ok = false;
                    else
                        code = (code << 4) | h;
                }
                if (ok) {
                    Text::_appendUtf8(out, code);
                    i += 4;
                }
                else // unrecognized sequence
                    out += "\\u";
                break;
            }
            default: // unrecognized escape
                out += '\\';
                out += next;
                break;
        }
    }
    return out;
}

int Text::_hexVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

void Text::_appendUtf8(std::string& out, u32 cp) {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    }
    else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
}

std::string Text::_format(const std::string& tpl, const std::vector<std::string>& args) {
    std::string out;
    out.reserve(tpl.size() + 32);

    for (size_t i = 0; i < tpl.size(); ++i) {
        char c = tpl[i];

        // Literal "{{" -> '{'
        if (c == '{' && i + 1 < tpl.size() && tpl[i + 1] == '{') {
            out += '{';
            ++i;
            continue;
        }

        // "{n}"
        if (c == '{') {
            size_t j = i + 1;
            size_t idx = 0;
            bool hasDigit = false;
            while (j < tpl.size() && tpl[j] >= '0' && tpl[j] <= '9') {
                idx = idx * 10 + (tpl[j] - '0');
                hasDigit = true;
                ++j;
            }

            if (hasDigit && j < tpl.size() && tpl[j] == '}') {
                if (idx < args.size())
                    out += args[idx];
                // else: Index out of range
                i = j; // Skip to the closing '}'
                continue;
            }
        }

        out += c;
    }
    return out;
}

std::string Text::_build(const std::string& key,
                            std::initializer_list<std::string> args,
                            const Color inColor,
                            const Color outColor) {
    std::string result;

    if (inColor.a > 0)
        result += static_cast<std::string>(inColor);

    if (_texts.contains(key))
        result += Text::_format(_texts.at(key), std::vector<std::string>(args));
    else
        result += "[" + key + "]"; // Fallback when text is not in txt

    if (outColor.a > 0)
        result += static_cast<std::string>(outColor);

    return result;
}

size_t Text::_utf8SeqLen(unsigned char c) {
    if (c < 0x80)          return 1;
    if ((c >> 5) == 0x06)  return 2;
    if ((c >> 4) == 0x0E)  return 3;
    if ((c >> 3) == 0x1E)  return 4;
    return 1;
}