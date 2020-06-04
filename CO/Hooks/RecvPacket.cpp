#include "RecvPacket.h"

using namespace CO::Hooks;

void CRecvPacket::_Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi)
{
    void* params[2] = { *(void**)(esp+4), *(void**)(esp+8) };
    _parent->OnRecvPacket(this, params);
}
