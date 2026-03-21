#include <CTRPluginFramework.hpp>
#include "GameHook.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
using namespace CTRPluginFramework;

void GameHook::init(std::vector<Monster>& monsters) {
    ProcessPlus::WriteFreeMem((u32)0); // bool for cheat enabled
    GameHook::_initBuyMonstersNotEnoughCoins();
    GameHook::_initmonstersCanBuy(monsters);
}

void GameHook::buyMonstersNotEnoughCoins(bool enable) {
    static bool firstTime = true;
    static std::array<u32, 4> backup = {0, 0, 0, 0};

    Addr addr = ARM::getAddr(ARM::AddrType::HOOK_NOT_ENOUGH_WIFI_COINS);

    // backup original jump if first time
    if (firstTime) {
        firstTime = false;

        for (int i = 0; i < 4; i++)
            Process::Read32(addr + i * 0x20, backup[i]);
    }

    for (int i = 0; i < 4; i++) {
        u32 val = enable ? ARM::getBranchInstruction(addr + i * 0x20, ProcessPlus::GetHookAddr(ARM::AddrType::HOOK_NOT_ENOUGH_WIFI_COINS) + i * 0xC) : backup[i];
        Process::Write32(addr + i * 0x20, val);
    }
}

void GameHook::monstersCanBuy(bool enable) {
    static u32 backup[2];
    static bool firstTime = true;

    static Addr addr[2] = {
        ARM::getAddr(ARM::AddrType::HOOK_MONSTERS_CAN_BUY_SHOW),
        ARM::getAddr(ARM::AddrType::HOOK_MONSTERS_CAN_BUY_FAMILIES)
    };

    // backup original jump if first time
    if (firstTime) {
        firstTime = false;
        Process::Read32(addr[0], backup[0]);
        Process::Read32(addr[1], backup[1]);
    }

    for (int i = 0; i < 2; i++) {
        ARM::AddrType type = i == 0 ? ARM::AddrType::HOOK_MONSTERS_CAN_BUY_SHOW : ARM::AddrType::HOOK_MONSTERS_CAN_BUY_FAMILIES;
        u32 val = enable ? ARM::getBranchInstruction(addr[i], ProcessPlus::GetHookAddr(type), true) : backup[i];
        Process::Write32(addr[i], val);
    }
}

void GameHook::buyEveryMonsterKind(bool enable) {
    if (!GameTitle::isJ3P())
        return;

    Addr addr = ARM::getAddr(ARM::AddrType::UNBUYABLE_MONSTERS);
    Process::Write8(addr, !enable);
}

void GameHook::_initBuyMonstersNotEnoughCoins() {
    // buy monsters if not enough coins
    Addr destAddr = ARM::getAddr(ARM::AddrType::HOOK_NOT_ENOUGH_WIFI_COINS);
    Addr writeAddr = ProcessPlus::GetNextFreeAddr();

    std::array<std::vector<u32>, 4> instr_buyMonstersNotEnoughCoins = {
        // mov r1, 0
        // cmp r1, r2
        // jump back
        std::vector<u32>{0xE3A01000, 0xE1510002, ARM::getBranchInstruction(writeAddr + 8, destAddr + 4)}, // bronze coins
        std::vector<u32>{0xE3A01000, 0xE1510002, ARM::getBranchInstruction(writeAddr + 0x14, destAddr + 0x24)}, // silver coins
        std::vector<u32>{0xE3A01000, 0xE1510002, ARM::getBranchInstruction(writeAddr + 0x20, destAddr + 0x44)}, // gold coins
        std::vector<u32>{0xE3A01000, 0xE1510002, ARM::getBranchInstruction(writeAddr + 0x2C, destAddr + 0x64)} // plat coins
    };

    for (std::vector<u32>& instructions : instr_buyMonstersNotEnoughCoins)
        ProcessPlus::WriteHook(instructions, {ARM::AddrType::HOOK_NOT_ENOUGH_WIFI_COINS});
}

void GameHook::_initmonstersCanBuy(std::vector<Monster>& monsters) {
    // patch addresses
    Addr dest = ProcessPlus::GetNextFreeAddr();

    Addr addr1 = ARM::getAddr(ARM::AddrType::HOOK_MONSTERS_CAN_BUY_SHOW);
    u32 branch1 = ARM::getBranchInstruction(addr1, dest, true);
    
    Addr addr2 = ARM::getAddr(ARM::AddrType::HOOK_MONSTERS_CAN_BUY_FAMILIES);
    u32 branch2 = ARM::getBranchInstruction(addr2, dest, true);

    Process::Write32(addr1, branch1);
    Process::Write32(addr2, branch2);

    // monsters to display in WiFi Coins shop
    std::vector<u32> instr_monstersCanBuy = {
        0xE55F203C, // ldrb r2, [pc, #-0x3C] // TODO automate by calculaing offset from current PC and 0x1E81000
        0xE3520000, // cmp r2, #0
        ARM::getBranchInstruction(ProcessPlus::GetNextFreeAddr() + sizeof(u32) - 8, ARM::getAddr(ARM::AddrType::HOOK_MONSTERS_CAN_BUY_ORIGINAL)) - 0xE0000000 - 3, // beq ORIGINAL_FUNC
        0xE1A0200F, // mov r2, pc
        0xE282202C, // add r2, r2, #0x2C
        0xE1D200B0, // ldrh r0, [r2]
        0xE3500000, // cmp r0, #0
        0x0A000003, // beq +=0x14
        0xE1500001, // cmp r0, r1
        0x0A000003, // beq +=0x14
        0xE2822002, // add r2, r2, #2
        0xEAFFFFF8, // b -=0x18
        0xE3A00000, // mov r0, #0
        0xE12FFF1E, // bx lr
        0xE3A00001, // mov r0, #1
        0xE12FFF1E  // bx lr
    };

    ProcessPlus::WriteHook(instr_monstersCanBuy, {
        ARM::AddrType::HOOK_MONSTERS_CAN_BUY_SHOW,
        ARM::AddrType::HOOK_MONSTERS_CAN_BUY_FAMILIES
    });

    // monster IDs
    for (Monster& monster : monsters)
        ProcessPlus::WriteFreeMem(monster.id);
}