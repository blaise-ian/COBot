#include "Talk.h"

using namespace CO::Hooks;

void CTalk::_Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi)
{
    Allow();
    void* params[] = {*(void**)(esp+4), *(void**)(esp+8), *(void**)(esp+12), *(void**)(esp+16), *(void**)(esp+20), *(void**)(esp+24)};
    _parent->OnTalk(this, params);
}
