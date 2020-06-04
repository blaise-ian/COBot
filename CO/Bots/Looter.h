#ifndef LOOTER_H_INCLUDED
#define LOOTER_H_INCLUDED

#include <windows.h>
#include "../Client.h"
#include <set>
#include <list>
#include <string>

namespace CO {
    class CClient;
namespace Bots {

    class CLooter
    {
    private:
        struct SCoord
        {
            int X, Y;
            SCoord(int x, int y): X(x), Y(y) {}
            bool operator==(const SCoord& rhs) const { return X == rhs.X && Y == rhs.Y; }
        };
        CClient*          _client;
        bool              _running;
        std::set<int>     _itemsToLoot;
        std::set<int>     _itemsToIgnore;
        std::list<SCoord> _recentlyDropped;
        DWORD             _nextTime;
        int               _dropIdToLoot;
        int               _xToLoot;
        int               _yToLoot;
        std::string       _baseDir;
        bool              _lootNormal;
        bool              _lootRefined;
        bool              _lootUnique;
        bool              _lootElite;
        bool              _lootSuper;
    protected:
        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CLooter(CClient* client, const char* baseDir);

        bool IsLooting;
    };

}
}

#endif // LOOTER_H_INCLUDED
