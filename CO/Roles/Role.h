#ifndef ROLE_H_INCLUDED
#define ROLE_H_INCLUDED

#include "../Addresses.h"

namespace CO {
namespace Roles {

    class CRole
    {
    protected:
        CAddresses* _addresses;
        int         _baseAddr;
    public:
        CRole(CAddresses* addresses, int baseAddr): _addresses(addresses), _baseAddr(baseAddr) {}
        void GetCoords(int* x, int* y);

        int UniqueId() const { return *(int*)(_baseAddr + 0x90); }
        int AnimationFlag() const { return *(unsigned char*)(_baseAddr + 0x60); }
        int BaseAddr() const { return _baseAddr; }
        const char* Name() const { return (char*)*(int*)(_baseAddr + 0x114); }
    };

}
}

#endif // ROLE_H_INCLUDED
