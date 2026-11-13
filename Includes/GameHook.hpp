#include <CTRPluginFramework.hpp>
#include "Content.hpp"
using namespace CTRPluginFramework;

class GameHook {
    static void _initBuyMonstersNotEnoughCoins(int coinIdx);
    static void _initmonstersCanBuy(std::vector<Monster>& monsters);
    
    public:
    static void init(std::vector<Monster>& monsters);
};