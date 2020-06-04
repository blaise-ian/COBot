#ifndef DICEKING_H_INCLUDED
#define DICEKING_H_INCLUDED

#include "../Addresses.h"
#include "../../Hook.h"
#include "../../Event.h"

namespace CO {
namespace Hooks {
    class CDiceKing: public CHook
    {
    protected:
        virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi)
        {
            void* params[] = {*(void**)(esp), *(void**)(esp + 4), *(void**)(esp + 8)};
            OnRole(this, params);
        }
    public:
        CDiceKing(CAddresses* addresses): CHook((void*)addresses->DiceKingHook, (void*)(addresses->DiceKingHook + 5))
        {
            SetFixUpCode("\xFF\x75\xF0\x8B\xC8", 5);
        }

        CEvent OnRole;
    };
}
}

#endif // DICEKING_H_INCLUDED
