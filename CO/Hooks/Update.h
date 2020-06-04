#ifndef UPDATE_H_INCLUDED
#define UPDATE_H_INCLUDED

#include <windows.h>
#include "../../Hook.h"
#include "../Addresses.h"
#include "../Client.h"

namespace CO {
    class CClient;
namespace Hooks {

    class CUpdate: public CHook
    {
    public:
        CClient* _parent;
    protected:
        virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi);
    public:
        CUpdate(CClient* parent, CAddresses* addresses): CHook((void*)addresses->UpdateHookAddr, (void*)(addresses->UpdateHookAddr+5)), _parent(parent)
        {
            DWORD old;
            VirtualProtect((void*)addresses->UpdateHookAddr, 5, PAGE_EXECUTE_READWRITE, &old);
            SetFixUpCode((char*)addresses->UpdateHookAddr, 5);
            VirtualProtect((void*)addresses->UpdateHookAddr, 5, old, &old);
        }
    };

}
}

#endif // UPDATE_H_INCLUDED
