#ifndef POTTER_H_INCLUDED
#define POTTER_H_INCLUDED

#include "../Client.h"

namespace CO {
    class CClient;
namespace Bots {

    class CPotter
    {
    private:
        CClient* _client;
        bool     _pottingHp;
        int      _hpLevel;
        int      _hpFKey;
        bool     _pottingMp;
        int      _mpLevel;
        int      _mpFKey;
        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
    public:
        CPotter(CClient* client);
    };

}
}

#endif // POTTER_H_INCLUDED
