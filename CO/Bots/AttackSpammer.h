#ifndef ATTACKSPAMMER_H_INCLUDED
#define ATTACKSPAMMER_H_INCLUDED

#include <windows.h>
#include "../Client.h"
#include <set>
#include <string>

namespace CO {
    class CClient;
namespace Bots {

    class CAttackSpammer
    {
    private:
        CClient* _client;
        std::set<int> _mobsToIgnore;
        std::set<std::string> _targets;
        DWORD _lastTime;
        int _px, _py;
        bool _first;

        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CAttackSpammer(CClient* client);
    };

}
}

#endif // ATTACKSPAMMER_H_INCLUDED
