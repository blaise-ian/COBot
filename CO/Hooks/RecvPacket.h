#ifndef RECVPACKET_H_INCLUDED
#define RECVPACKET_H_INCLUDED

#include <windows.h>
#include "../../Hook.h"
#include "../Client.h"
#include "../Addresses.h"

namespace CO {
    class CClient;
namespace Hooks {

    class CRecvPacket: public CHook
    {
    private:
        CClient* _parent;
    protected:
        virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi);
    public:
        CRecvPacket(CClient* parent, CAddresses* addresses):
            CHook((void*)addresses->RecvPacketHook, (void*)(addresses->RecvPacketHook+5)),
            _parent(parent)
        {
            DWORD old;
            VirtualProtect((void*)addresses->RecvPacketHook, 5, PAGE_EXECUTE_READWRITE, &old);
            SetFixUpCode((char*)addresses->RecvPacketHook, 5);
            VirtualProtect((void*)addresses->RecvPacketHook, 5, old, &old);
        }
    };

}
}

#endif // RECVPACKET_H_INCLUDED
