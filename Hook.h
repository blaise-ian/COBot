#ifndef HOOK_H_INCLUDED
#define HOOK_H_INCLUDED

#include <windows.h>

class CHook
{
private:
    void* _addr;
    void* _retAddr;
    char  _patch[5];
    char  _unpatch[5];
    char* _cave;
    int   _caveSize;
    char* _fixUpCode;
    int   _fixUpCodeSize;
    DWORD _oldCavePermissions;
    DWORD _oldAddrPermissions;
    DWORD _oldFixUpCodePermissions;
    DWORD _oldPatchPermissions;
    int   _regs[8];
    static void _SCallback(CHook* _this);
protected:
    virtual void _Callback(int& eax, int& ecx, int& edx, int& ebx, int& esp, int& ebp, int& esi, int& edi) = 0;
public:
    CHook(void* addr, void* retAddr);
    virtual ~CHook();

    void SetFixUpCode(char* code, int size);
    void Patch();
    void Unpatch();
};

#endif // HOOK_H_INCLUDED
