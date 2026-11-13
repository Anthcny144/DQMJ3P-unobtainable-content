#include <CTRPluginFramework.hpp>
#include "GameHook.hpp"
#include "Macro.hpp"
#include "Memory.hpp"
using namespace CTRPluginFramework;

void GameHook::init(std::vector<Monster>& monsters) {
    ProcessPlus::WriteFreeMem((u32)0); // Bool for cheat enabled
    for (int i = 0; i < 4; i++)
        GameHook::_initBuyMonstersNotEnoughCoins(i);
    GameHook::_initmonstersCanBuy(monsters);
}

void GameHook::_initBuyMonstersNotEnoughCoins(int coinIdx) {
    u32 funcOffset = 0x20;

    Addr hookAddr = ARM::getGameHook(ARM::HookType::NOT_ENOUGH_WIFI_COINS) + coinIdx * funcOffset;
    Addr destAddr = hookAddr + 4;
    Addr entryAddr = ProcessPlus::GetNextFreeAddr();
    Addr writeAddr = entryAddr + 0x10;
    u32 ldrbFlag = 0xE55F0000 | (entryAddr + 8 - Macro::FREE_RAM_SPACE_ADDR);

    std::vector<u32> instr_buyMonstersNotEnoughCoins = {
        ldrbFlag,   // ldrb r0, [flag]
        0xE3500001, // cmp r0, #1
        0x03A01000, // moveq r1, #0
        0xE1510002, // cmp r1, r2
        ARM::getBranchInstruction(writeAddr, destAddr)
    };

    // Write hook
    ProcessPlus::WriteHook(instr_buyMonstersNotEnoughCoins, {ARM::HookType::NOT_ENOUGH_WIFI_COINS});

    // Write jump
    Process::Write32(hookAddr, ARM::getBranchInstruction(hookAddr, entryAddr));
}

void GameHook::_initmonstersCanBuy(std::vector<Monster>& monsters) {
    // Patch addresses
    Addr dest = ProcessPlus::GetNextFreeAddr();
    Addr addr1 = ARM::getGameHook(ARM::HookType::MONSTERS_CAN_BUY_SHOW);
    u32 branch1 = ARM::getBranchInstruction(addr1, dest, true);
    Addr addr2 = ARM::getGameHook(ARM::HookType::MONSTERS_CAN_BUY_FAMILIES);
    u32 branch2 = ARM::getBranchInstruction(addr2, dest, true);

    Process::Write32(addr1, branch1);
    Process::Write32(addr2, branch2);

    // Get ldrb and beq instructions
    Addr entry = ProcessPlus::GetNextFreeAddr();
    Addr origFunc = ARM::getGameHook(ARM::HookType::MONSTERS_CAN_BUY_ORIGINAL);
    u32 ldrbFlag = 0xE55F2000 | (entry + 8 - Macro::FREE_RAM_SPACE_ADDR);
    u32 beqOrig = (ARM::getBranchInstruction(entry + 8, origFunc) & 0xFFFFFF) | 0x0A000000;

    // Monsters to display in WiFi Coins shop
    std::vector<u32> instr_monstersCanBuy = {
        ldrbFlag, // ldrb r2, [flag]
        0xE3520000, // cmp r2, #0
        beqOrig, // beq ORIGINAL_FUNC
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
        ARM::HookType::MONSTERS_CAN_BUY_SHOW,
        ARM::HookType::MONSTERS_CAN_BUY_FAMILIES
    });

    // Monster IDs
    for (Monster& monster : monsters)
        ProcessPlus::WriteFreeMem(monster.id);
}