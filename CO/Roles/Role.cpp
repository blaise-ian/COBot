#include "Role.h"

using namespace CO::Roles;

void CRole::GetCoords(int* x, int* y)
{
    __asm("pushal");
    __asm
    (
        "pushl %%edx\n\t"
        "pushl $0\n\t"
        "pushl $0\n\t"
        "pushl %%esp\n\t"
        "call *0x14(%%eax)\n\t"
        "popl %%eax\n\t"
        "popl %%ecx\n\t"
        "popl %%edx\n\t"
        : "=a"(*x), "=c"(*y) : "a"(*(int*)_baseAddr), "c"(_baseAddr) :
    );
    __asm("popal");
}
