#ifndef MEMORYSEARCHER_H_INCLUDED
#define MEMORYSEARCHER_H_INCLUDED

#include <windows.h>

class CMemorySearcher
{
private:
    HANDLE _hProcess;
    char*  _buffer;
    int    _start;
    int    _end;
public:
    CMemorySearcher(int start, int end);
    ~CMemorySearcher();

    bool Check(const char* data, int dataSize, int addr);
    int Find(const char* data, int dataSize);
    int ReadInt32(int addr);
};

#endif // MEMORYSEARCHER_H_INCLUDED
