#ifndef BOTJAIL_H_INCLUDED
#define BOTJAIL_H_INCLUDED

#include "../Client.h"

namespace CO {
    class CClient;
namespace Bots {

    class CBotJail
    {
    private:
        CClient* _client;
        bool     _running;
        int      _case;

        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CBotJail(CClient* client);
    };

}
}

#endif // BOTJAIL_H_INCLUDED
