#ifndef GAMBLER_H_INCLUDED
#define GAMBLER_H_INCLUDED

#include <windows.h>
#include "../Client.h"

namespace CO {
    class CClient;
namespace Bots {

    class CGambler
    {
    private:
        CClient*    _client;
        bool        _running;
        bool        _betMade;
        DWORD       _lastRoleTime;
        int         _history[16];
        int         _cell;
        int         _amount;
        std::string _baseDir;

        void _Role(void* sender, void** params);
        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CGambler(CClient* client, const char* dir);
    };

}
}

#endif // GAMBLER_H_INCLUDED
