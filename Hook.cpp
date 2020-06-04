#include "Hook.h"

/*
$-2      >   EB 14          JMP SHORT notepad.00579F66
$ ==>    >   0000           ADD BYTE PTR DS:[EAX],AL                 ;  _this pointer
$+2      >   0000           ADD BYTE PTR DS:[EAX],AL
$+4      >   0000           ADD BYTE PTR DS:[EAX],AL                 ;  saved registers pointer
$+6      >   0000           ADD BYTE PTR DS:[EAX],AL
$+8      >   0000           ADD BYTE PTR DS:[EAX],AL                 ;  _fixUpCodePtr
$+A      >   0000           ADD BYTE PTR DS:[EAX],AL
$+C      >   0000           ADD BYTE PTR DS:[EAX],AL                 ;  CallbackFnPtr
$+E      >   0000           ADD BYTE PTR DS:[EAX],AL
$+10     >   0000           ADD BYTE PTR DS:[EAX],AL                 ;  temporary storage
$+12     >   0000           ADD BYTE PTR DS:[EAX],AL
$+14     >   60             PUSHAD
$+15     >   E8 00000000    CALL notepad.00579F6C
$+1A     >   58             POP EAX
$+1B     >   83E8 1A        SUB EAX,1A
$+1E     >   8B48 04        MOV ECX,DWORD PTR DS:[EAX+4]
$+21     >   8948 5E        MOV DWORD PTR DS:[EAX+5E],ECX
$+24     >   83C1 20        ADD ECX,20
$+27     >   8948 47        MOV DWORD PTR DS:[EAX+47],ECX
$+2A     >   8D4C24 20      LEA ECX,DWORD PTR SS:[ESP+20]
$+2E     >   8948 4D        MOV DWORD PTR DS:[EAX+4D],ECX
$+31     >   8948 64        MOV DWORD PTR DS:[EAX+64],ECX
$+34     >   8B08           MOV ECX,DWORD PTR DS:[EAX]
$+36     >   8948 52        MOV DWORD PTR DS:[EAX+52],ECX
$+39     >   8B48 0C        MOV ECX,DWORD PTR DS:[EAX+C]
$+3C     >   8948 57        MOV DWORD PTR DS:[EAX+57],ECX
$+3F     >   8B48 08        MOV ECX,DWORD PTR DS:[EAX+8]
$+42     >   8948 69        MOV DWORD PTR DS:[EAX+69],ECX
$+45     >   61             POPAD
$+46     >   BC 88888888    MOV ESP,88888888
$+4B     >   60             PUSHAD
$+4C     >   BC 88888888    MOV ESP,88888888
$+51     >   68 88888888    PUSH 88888888
$+56     >   B8 88888888    MOV EAX,88888888
$+5B     >   FFD0           CALL EAX
$+5D     >   BC 88888888    MOV ESP,88888888
$+62     >   61             POPAD
$+63     >   BC 88888888    MOV ESP,88888888
$+68     >   68 88888888    PUSH 88888888
$+6D     >   C3             RETN

EB 14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 60 E8 00 00 00 00 58 83 E8 1A
8B 48 04 89 48 5E 83 C1 20 89 48 47 8D 4C 24 20 89 48 4D 89 48 64 8B 08 89 48 52 8B 48 0C 89 48
57 8B 48 08 89 48 69 61 BC 88 88 88 88 60 BC 88 88 88 88 68 88 88 88 88 B8 88 88 88 88 FF D0 BC
88 88 88 88 61 BC 88 88 88 88 68 88 88 88 88 C3
*/

void CHook::_SCallback(CHook* _this)
{
    _this->_regs[3] = *(int*)&(_this->_cave[0x4F]);
    _this->_Callback(_this->_regs[7], _this->_regs[6], _this->_regs[5], _this->_regs[4], _this->_regs[3], _this->_regs[2], _this->_regs[1], _this->_regs[0]);
    *(int*)&(_this->_cave[0x4F]) = _this->_regs[3];
}

CHook::CHook(void* addr, void* retAddr): _addr(addr), _retAddr(retAddr), _cave(0), _caveSize(0), _fixUpCode(0), _fixUpCodeSize(0)
{
    int tmp;
    _fixUpCode = new char[5];
    tmp = (int)_retAddr - ((int)_fixUpCode + 5);
    _fixUpCode[0] = 0xE9;
    _fixUpCode[1] = ((char*)&tmp)[0];
    _fixUpCode[2] = ((char*)&tmp)[1];
    _fixUpCode[3] = ((char*)&tmp)[2];
    _fixUpCode[4] = ((char*)&tmp)[3];
    _fixUpCodeSize = 5;
    VirtualProtect(_fixUpCode, _fixUpCodeSize, PAGE_EXECUTE_READWRITE, &_oldFixUpCodePermissions);

    char baseCode[] = {0xEB, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x58, 0x83, 0xE8, 0x1A,
                       0x8B, 0x48, 0x04, 0x89, 0x48, 0x5E, 0x83, 0xC1, 0x20, 0x89, 0x48, 0x47, 0x8D, 0x4C, 0x24, 0x20, 0x89, 0x48, 0x4D, 0x89, 0x48, 0x64, 0x8B, 0x08, 0x89, 0x48, 0x52, 0x8B, 0x48, 0x0C, 0x89, 0x48,
                       0x57, 0x8B, 0x48, 0x08, 0x89, 0x48, 0x69, 0x61, 0xBC, 0x88, 0x88, 0x88, 0x88, 0x60, 0xBC, 0x88, 0x88, 0x88, 0x88, 0x68, 0x88, 0x88, 0x88, 0x88, 0xB8, 0x88, 0x88, 0x88, 0x88, 0xFF, 0xD0, 0xBC,
                       0x88, 0x88, 0x88, 0x88, 0x61, 0xBC, 0x88, 0x88, 0x88, 0x88, 0x68, 0x88, 0x88, 0x88, 0x88, 0xC3};
    _cave = new char[sizeof(baseCode)];
    memcpy(_cave, baseCode, sizeof(baseCode));
    *(int*)&_cave[2]  = (int)this;
    *(int*)&_cave[6]  = (int)_regs;
    *(int*)&_cave[10] = (int)_fixUpCode;
    *(int*)&_cave[14] = (int)_SCallback;
    VirtualProtect(_cave, _caveSize, PAGE_EXECUTE_READWRITE, &_oldCavePermissions);

    tmp = (int)_cave - ((int)addr + 5);
    _patch[0] = 0xE9;
    _patch[1] = ((char*)&tmp)[0];
    _patch[2] = ((char*)&tmp)[1];
    _patch[3] = ((char*)&tmp)[2];
    _patch[4] = ((char*)&tmp)[3];

    VirtualProtect(_addr, 5, PAGE_EXECUTE_READWRITE, &_oldPatchPermissions);
    memcpy(_unpatch, _addr, 5);
}

CHook::~CHook()
{
    Unpatch();
    if (_fixUpCode)
    {
        VirtualProtect(_fixUpCode, _fixUpCodeSize, _oldFixUpCodePermissions, &_oldFixUpCodePermissions);
        delete[] _fixUpCode;
    }
    else
        return;
    if (_cave)
    {
        VirtualProtect(_cave, _caveSize, _oldCavePermissions, &_oldCavePermissions);
        delete[] _cave;
    }
    else
        return;
    VirtualProtect(_addr, 5, _oldPatchPermissions, &_oldPatchPermissions);
}

void CHook::SetFixUpCode(char* code, int size)
{
    if (_fixUpCode)
    {
        VirtualProtect(_fixUpCode, _fixUpCodeSize, _oldFixUpCodePermissions, &_oldFixUpCodePermissions);
        delete[] _fixUpCode;
    }
    _fixUpCode = 0;
    _fixUpCodeSize = 0;
    _fixUpCode = new char[size + 5];
    *(int*)&_cave[10] = (int)_fixUpCode;
    *(int*)&_cave[0x6B] = (int)_fixUpCode;
    memcpy(_fixUpCode, code, size);
    int tmp = (int)_retAddr - ((int)_fixUpCode + size + 5);
    _fixUpCode[size] = 0xE9;
    _fixUpCode[size+1] = ((char*)&tmp)[0];
    _fixUpCode[size+2] = ((char*)&tmp)[1];
    _fixUpCode[size+3] = ((char*)&tmp)[2];
    _fixUpCode[size+4] = ((char*)&tmp)[3];
    _fixUpCodeSize = size + 5;
    VirtualProtect(_fixUpCode, _fixUpCodeSize, PAGE_EXECUTE_READWRITE, &_oldFixUpCodePermissions);
}

void CHook::Patch()
{
    memcpy(_addr, _patch, 5);
}

void CHook::Unpatch()
{
    memcpy(_addr, _unpatch, 5);
}
