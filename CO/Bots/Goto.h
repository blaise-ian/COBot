#ifndef GOTO_H_INCLUDED
#define GOTO_H_INCLUDED

#include <windows.h>
#include "../Client.h"
#include "../Path/PathFinder.h"

namespace CO {
    class CClient;
    namespace Path
    {
        class CPathFinder;
    }
namespace Bots {

    class CGoto
    {
    private:
        CClient*           _client;
        Path::CPathFinder* _pathFinder;
        bool               _running;
        DWORD              _lastTime;
        DWORD              _delay;

        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CGoto(CClient* client);
        ~CGoto();

        bool Done() const { return !_running; }
        void Goto(int x, int y, int delayStart = 0);
    };

}
}

#endif // GOTO_H_INCLUDED
