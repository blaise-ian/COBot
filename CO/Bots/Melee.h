#ifndef MELEE_H_INCLUDED
#define MELEE_H_INCLUDED

#include <windows.h>
#include "../Client.h"
#include <set>
#include <vector>
#include <string>

namespace CO {
    class CClient;
namespace Bots {

    class CMelee
    {
    private:
        struct SCoord
        {
            int X, Y;
            SCoord(int x, int y): X(x), Y(y) {}
            bool operator<(const SCoord& rhs) const
            {
                return X < rhs.X || (X == rhs.X && (Y < rhs.Y));
            }
        };
        CClient* _client;
        bool     _running;
        std::set<std::string> _targets;
        std::set<SCoord>      _spawnSpots;
        std::set<int>         _mobsToIgnore;
        DWORD    _nextTime;
        bool     _firstJump;
        std::vector<SCoord> _wayPoints;
        int                 _atWayPoint;
        // for fatal strike
        bool _first;
        int _px, _py;

        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CMelee(CClient* client);
    };

}
}

#endif // MELEE_H_INCLUDED
