#ifndef FOLLOWER_H_INCLUDED
#define FOLLOWER_H_INCLUDED

#include "../Client.h"
#include <windows.h>
#include <string>

namespace CO {
    class CClient;
namespace Bots {
    class CFollower
    {
    private:
        CClient*    _client;
        bool        _running;
        std::string _target;
        DWORD       _lastTime;

        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CFollower(CClient* client);
    };
}
}

#endif // FOLLOWER_H_INCLUDED
