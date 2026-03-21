#include <CTRPluginFramework.hpp>
#include "CTRPFExtension.hpp"
#include "Macro.hpp"
using namespace CTRPluginFramework;

Addr ProcessPlus::_addr = Macro::FREE_RAM_SPACE_ADDR;
std::unordered_map<ARM::AddrType, Addr> ProcessPlus::_hookAddrs = {};

void ProcessPlus::ClearFreeMem() {
    static u8 buffer[Macro::FREE_RAM_SPACE_SIZE] = {};
    Process::CopyMemory((void *)Macro::FREE_RAM_SPACE_ADDR, buffer, Macro::FREE_RAM_SPACE_SIZE);
    _addr = Macro::FREE_RAM_SPACE_ADDR;
}

void ProcessPlus::WriteHook(const std::vector<u32>& instructions, std::initializer_list<ARM::AddrType> types) {
    for (ARM::AddrType type : types)
        _hookAddrs[type] = _addr;

    for (u32 instruction : instructions)
        ProcessPlus::WriteFreeMem(instruction);
}

Addr ProcessPlus::GetHookAddr(ARM::AddrType type) {
    if (_hookAddrs.contains(type))
        return _hookAddrs[type];

    return 0;
}

Addr ProcessPlus::GetNextFreeAddr() {
    return _addr;
}

bool MessageBoxPlus::wrap(const std::string& title, const std::string& msg, DialogType dialogue, ClearScreen clear) {
    Sleep(Milliseconds(100));
    bool res = MessageBox(title, msg, dialogue, clear)();
    Sleep(Milliseconds(100));
    return res;
}

CustomIcon CustomIconPlus::loadIcon(const std::string& path) {
    bool fileOk = true;

    u32 x = CustomIconPlus::size,
        y = CustomIconPlus::size,
        iconSize = x * y,
        bufferSize = iconSize * sizeof(CustomIcon::Pixel);

    File binary;
    fileOk = File::Open(binary, path, File::READ) == 0;

    CustomIcon::Pixel* pixelArray = new CustomIcon::Pixel[iconSize];
    if (fileOk)
        fileOk = binary.Read(pixelArray, bufferSize) == 0;

    return CustomIcon(pixelArray,
                      fileOk ? x : 0,
                      fileOk ? y : 0,
                      true);
}

void CustomIconPlus::draw(const Screen& screen, CustomIcon& icon, int drawX, int drawY, unsigned int scale) {
    const int maxWidth = screen.IsTop ? Macro::SCREEN_TOP_WIDTH : Macro::SCREEN_BOT_WIDTH;

    for (int y = 0; y < CustomIconPlus::size; y++) {
        for (int x = 0; x < CustomIconPlus::size; x++) {
            u32 pxIdx = x * CustomIconPlus::size + (CustomIconPlus::size - 1 - y);
            CustomIcon::Pixel px = icon.pixArray[pxIdx];
            int posX = drawX + x * scale;
            int posY = drawY + y * scale;

            if (px.a > 25 && posX >= 0 && posX < maxWidth && posY >= 0 && posY < Macro::SCREEN_HEIGHT)
                screen.DrawPixel(drawX + x * scale, drawY + y * scale, Color(px.r, px.g, px.b, px.a));
        }
    }
}

Gradient::Gradient(Color mainColor) : first(mainColor) {}
Gradient::Gradient(Color first, Color second, float power) : first(first), second(second)
{
    this->power = std::clamp(power, (float)0, (float)1);
}

const Gradient Gradient::Bronze = Gradient(Color(0xCC8844FF), Color::White, 0.75f),
               Gradient::Silver = Gradient(Color(0xBBCCDDFF), Color::White, 0.75f),
               Gradient::Gold = Gradient(Color(0xFFFF00FF), Color::White, 0.75f),
               Gradient::Platinum = Gradient(Color(0x0FCFFFFF), Color::White);
