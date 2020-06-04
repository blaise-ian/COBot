#include "MemorySearcher.h"


CMemorySearcher::CMemorySearcher(int start, int end): _start(start), _end(end)
{
    _hProcess = OpenProcess(PROCESS_ALL_ACCESS, true, GetCurrentProcessId());
    //MessageBox(0, "allocating buffer", "info", MB_OK);
    _buffer = new char[end - start + 1];
    //MessageBox(0, "copying memory", "info", MB_OK);
    int totalRead = 0;
    int size = end - start + 1;
    while (totalRead < size)
    {
        int read = size - totalRead;
        if (read > 100)
            read = 100;
        ReadProcessMemory(_hProcess, (void*)(start + totalRead), &_buffer[totalRead], read, 0);
        totalRead += read;
    }
    //MessageBox(0, "memory copied", "info", MB_OK);
}

CMemorySearcher::~CMemorySearcher()
{
    delete[] _buffer;
    CloseHandle(_hProcess);
}

bool CMemorySearcher::Check(const char* data, int dataSize, int addr)
{

    //char buff[dataSize];
    //ReadProcessMemory(_hProcess, (void*)addr, buff, dataSize, 0);
    //return memcmp(data, buff, dataSize) == 0;

    return memcmp(data, &_buffer[addr - _start], dataSize) == 0;
}

int CMemorySearcher::Find(const char* data, int dataSize)
{
    //MessageBox(0, "doing search", "info", MB_OK);
    for (int i = _start; i <= _end - dataSize; ++i)
    {
        if (Check(data, dataSize, i))
        //if (memcmp(data, &_buffer[i - _start], dataSize) == 0)
            return i;
    }
    //MessageBox(0, "not found", "info", MB_OK);
    return 0;
}

int CMemorySearcher::ReadInt32(int addr)
{
    int x;
    ReadProcessMemory(_hProcess, (void*)addr, &x, 4, 0);
    return x;
}
