#include "Update.h"

using namespace CO::Hooks;

void CUpdate::_Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi)
{
    _parent->OnUpdate(this, 0);
}
