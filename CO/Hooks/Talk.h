#ifndef TALK_H_INCLUDED
#define TALK_H_INCLUDED

#include <windows.h>
#include "../../Hook.h"
#include "../Addresses.h"
#include "../Roles/Hero.h"

namespace CO {
    namespace Roles {
        class CHero;
    }
namespace Hooks {

    class CTalk: public CHook
    {
    private:
        bool          _isAllow;
        Roles::CHero* _parent;
        char          _fixUpCode[5];
    protected:
        virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi);
    public:
        CTalk(Roles::CHero* parent, CAddresses* addresses): CHook((void*)addresses->TalkFn, (void*)(addresses->TalkFn+5)), _isAllow(true), _parent(parent)
        {
            DWORD old;
            VirtualProtect((void*)addresses->TalkFn, 5, PAGE_EXECUTE_READWRITE, &old);
            memcpy(_fixUpCode, (void*)addresses->TalkFn, 5);
            VirtualProtect((void*)addresses->TalkFn, 5, old, &old);
            Allow();
        }

        void Allow()
        {
            if (!_isAllow)
            {
                SetFixUpCode(_fixUpCode, 5);
                _isAllow = true;
            }
        }

        void Block()
        {
            if (_isAllow)
            {
                SetFixUpCode("\xC2\x18\x00", 3);
                _isAllow = false;
            }
        }
    };

}
}

#endif // TALK_H_INCLUDED
