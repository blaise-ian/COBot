#include <stdlib.h>
#include <windows.h>
#include "SecretCall.h"

CSecretCall::CSecretCall(int tableSize, int codeSize):
    _tableSize(tableSize),
    _codeSize(codeSize)
{
    _table = new char*[_tableSize];
    for (int i = 0; i < _tableSize; ++i)
    {
        _table[i] = new char[_codeSize];
        for (int j = 0; j < _codeSize; ++j)
            _table[i][j] = rand() % 0xFF;
    }
}

CSecretCall::~CSecretCall()
{
    for (int i = 0; i < _tableSize; ++i)
    {
        delete[] _table[i];
    }
    delete[] _table;
}

void CSecretCall::SetCode(int key, char* code)
{
    int i = key % _tableSize;
    for (int j = 0; j < _codeSize; ++j)
        _table[i][j] = code[j];
}

void CSecretCall::operator()(int key, void* param)
{
    int fnAddr = (int)(_table[key % _tableSize]);
    DWORD old;
    VirtualProtect((void*)fnAddr, _codeSize, PAGE_EXECUTE_READWRITE, &old);
    __asm
    (
        "pushl %%ecx\n\t"
        "call *%%eax"
        : : "a"(fnAddr), "c"(param) :
    );
    VirtualProtect((void*)fnAddr, _codeSize, old, &old);
}
