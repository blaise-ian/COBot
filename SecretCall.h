#ifndef SECRETCALL_H_INCLUDED
#define SECRETCALL_H_INCLUDED


class CSecretCall
{
private:
    int    _tableSize;
    int    _codeSize;
    char** _table;
public:
    CSecretCall(int tableSize=1000, int codeSize=20);
    ~CSecretCall();

    void SetCode(int key, char* code);
    void operator()(int key, void* param);
};

#endif // SECRETCALL_H_INCLUDED
