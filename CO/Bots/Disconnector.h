#ifndef DISCONNECTOR_H_INCLUDED
#define DISCONNECTOR_H_INCLUDED

#include "../Client.h"

namespace CO {
    class CClient;
namespace Bots {

    class CDisconnector
    {
    private:
        CClient* _client;
        bool     _running;
        bool     _dcOnBlack;
        bool     _dcOnRed;
        bool     _dcOnBlue;
        bool     _dcOnGm;
        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CDisconnector(CClient* client);
    };

}
}

#endif // DISCONNECTOR_H_INCLUDED
