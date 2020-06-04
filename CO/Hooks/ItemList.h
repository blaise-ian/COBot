#ifndef ITEMLIST_H_INCLUDED
#define ITEMLIST_H_INCLUDED

#include <windows.h>
#include "../../Hook.h"

namespace CO {
namespace Hooks {

    class CItemList: public CHook
    {
    private:
        int         _basePtr;
    protected:
        virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi)
        {
            _basePtr = ecx + 4;
        }
    public:
        CItemList(CAddresses* addresses):
            CHook((void*)addresses->GroundItemListHook, (void*)(addresses->GroundItemListHook+5)),
            _basePtr(0)
        {
            DWORD old;
            VirtualProtect((void*)addresses->GroundItemListHook, 5, PAGE_EXECUTE_READWRITE, &old);
            SetFixUpCode((char*)addresses->GroundItemListHook, 5);
            VirtualProtect((void*)addresses->GroundItemListHook, 5, old, &old);
        }

        int Count()
        {
            if (_basePtr)
                return (*(int*)(_basePtr+4) - *(int*)_basePtr) >> 2;
            else
                return 0;
        }

        void Get(int i, int* dropId, int* itemId, int* x, int* y)
        {
            if (_basePtr)
            {
                int base = *(int*)_basePtr;
                int itemPtr = ((int*)base)[i];
                *dropId = ((int*)itemPtr)[0];
                *itemId = ((int*)itemPtr)[1];
                *x      = ((int*)itemPtr)[2];
                *y      = ((int*)itemPtr)[3];
            }
            else
            {
                *dropId = *itemId = *x = *y = 0;
            }
        }
    };

}
}

#endif // ITEMLIST_H_INCLUDED
