#pragma once
#include <CTRPluginFramework.hpp>
#include "Macro.hpp"
#include "Memory.hpp"
#include <initializer_list>
#include <unordered_map>
using namespace CTRPluginFramework;

struct ProcessPlus {
    /**
     * @brief Writes an integer value to the next free CTRPF memory space
     * @param value The value to write (s8, u8, s16, u16, s32, u32, s64, u64)
     * @param align Whether to align the write address on the value's size
     */
    template<typename T>
    static inline bool WriteFreeMem(T value, bool align = true) {
        static_assert(std::is_integral_v<T>, "ProcessPlus::WriteFreeMem can only write integer values");

        static bool full = false;

        // align
        if (align)
            _addr = (_addr + sizeof(T) - 1) & ~(sizeof(T) - 1);

        // if no free space
        if (_addr + sizeof(T) > Macro::FREE_RAM_SPACE_ADDR + Macro::FREE_RAM_SPACE_SIZE) {
            if (!full) {
                OSD::Notify(Color::Red << "ERROR: " << Color::Silver << "CTRPF free mem is full!");
                full = true;
            }
            return false;
        }

        bool res;

        // write
        if constexpr (sizeof(T) == 1)
            res = Process::Write8(_addr, value);
        else if constexpr (sizeof(T) == 2)
            res = Process::Write16(_addr, value);
        else if constexpr (sizeof(T) == 4)
            res = Process::Write32(_addr, value);
        else if constexpr (sizeof(T) == 8)
            res = Process::Write64(_addr, value);

        _addr += sizeof(T);
        return res;
    }

    static void ClearFreeMem();
    static void WriteHook(const std::vector<u32>& instructions, std::initializer_list<ARM::AddrType> types);
    static Addr GetHookAddr(ARM::AddrType type);
    static Addr GetNextFreeAddr();

    private:
    static Addr _addr;
    static std::unordered_map<ARM::AddrType, Addr> _hookAddrs;
};

struct MessageBoxPlus {
    static bool wrap(const std::string& title, const std::string& msg, DialogType dialogue = DialogType::DialogOk, ClearScreen clear = ClearScreen::Both);
};

struct CustomIconPlus {
    static const u32 size = 30;

    static CustomIcon loadIcon(const std::string& path);
    static void draw(const Screen& screen, CustomIcon& icon, int drawX, int drawY, unsigned int scale = 1);
};

namespace ColorPlus {
    static const Color Pink = Color(0xFF84EBFF);
};

struct Gradient {
    Color first, second;
    float power = -1;

    Gradient(Color mainColor);
    Gradient(Color first, Color second, float power = 0.5f);
    
    static const Gradient Bronze, Silver, Gold, Platinum;
};