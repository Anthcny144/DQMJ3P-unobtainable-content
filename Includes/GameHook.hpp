#include <CTRPluginFramework.hpp>
#include "Content.hpp"
using namespace CTRPluginFramework;

struct GameHook {
    static void init(std::vector<Monster>& monsters);
    static void buyMonstersNotEnoughCoins(bool enable);
    static void monstersCanBuy(bool enable);
    static void buyEveryMonsterKind(bool enable);

    private:
    static void _initBuyMonstersNotEnoughCoins();
    static void _initmonstersCanBuy(std::vector<Monster>& monsters);
};