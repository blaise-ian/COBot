#ifndef XPSKILL_H_INCLUDED
#define XPSKILL_H_INCLUDED

#include "../Client.h"

namespace CO {
    class CClient;
namespace Bots {

    class CXpSkill
    {
    private:
        CClient* _client;
        bool     _running;
        int      _skillId;
        int      _lastTime;
        int      _fatalStrikeTime;
        void _Update(void* sender, void** params);
        void _Command(void* sender, void** params);
        void _RecvPacket(void* sender, void** params);
    public:
        CXpSkill(CClient* client);

        bool IsFatalStrikeOn;
    };

}
}

#endif // XPSKILL_H_INCLUDED
