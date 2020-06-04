#include "Client.h"

#include <math.h>
#include <sstream>
#include <string>
using namespace std;
using namespace CO;

void CClient::_DiceKingRole(void* sender, void** params)
{
    //int d1 = (int)(params[0]);
    //int d2 = (int)(params[1]);
    //int d3 = (int)(params[2]);
    //ostringstream os;
    //os << "DiceKing roles " << d1 << " " << d2 << " " << d3;
    //MessageBox(0, os.str().c_str(), "Info", MB_OK);
    // 100 on small if its small.
    /*
    if (d1 + d2 + d3 <= 10)
    {
        char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00};
        SendPacket(packet, sizeof(packet));
    }
    */
    OnDiceKingRole(this, params);
}

static DWORD WINAPI _EjectThread(LPVOID param)
{
    Sleep(1000);
    FreeLibraryAndExitThread(GetModuleHandle("Melee.dll"), 0);
}

void CClient::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg[0] == '#')
        Hero()->BlockTalk();
    if (msg == "#eject")
    {
        CreateThread(0, 0, _EjectThread, 0, 0, 0);
        Hero()->BlockTalk();
    }
    else if (msg == "#nummobs")
    {
        ostringstream os;
        os << "There are " << NumMobs() << " mobs on the screen.";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        Hero()->BlockTalk();
    }
    else if (msg.find("#getmob") == 0)
    {
        string cmd;
        int i;
        istringstream is;
        is >> cmd >> i;
        int x, y;
        Roles::CRole* role = GetMob(i);
        role->GetCoords(&x, &y);
        ostringstream os;
        os << "Mob " << i << " is located at (" << x << ", " << y << ").";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        delete role;
        Hero()->BlockTalk();
    }
    else if (msg.find("#melee1stmob") == 0)
    {
        Roles::CRole* role = GetMob(0);
        Hero()->Melee(role);
        delete role;
        Hero()->BlockTalk();
    }
    else if (msg.find("#readground") == 0)
    {
        istringstream is(msg);
        string cmd;
        int x, y;
        is >> cmd >> x >> y;
        int g = ReadGround(x, y);
        ostringstream os;
        os << "The ground at position (" << x << ", " << y << ") has the value " << g;
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        Hero()->BlockTalk();
    }
    else if (msg.find("#hero") == 0)
    {
        int px, py;
        Hero()->GetCoords(&px, &py);
        ostringstream os;
        os << "Your hero is positioned at (" << px << ", " << py << ")";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        Hero()->BlockTalk();
    }
    else if (msg.find("#numitems") == 0)
    {
        int num = _itemListHook.Count();
        ostringstream os;
        os << "There are " << num << " items detected on the ground.";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        Hero()->BlockTalk();
    }
    else if (msg.find("#getitem") == 0)
    {
        string cmd;
        int idx;
        istringstream is(msg);
        is >> cmd >> idx;
        int x, y, tmp;
        _itemListHook.Get(idx, &tmp, &tmp, &x, &y);
        ostringstream os;
        os << "Item " << idx << " is found at (" << x << ", " << y << ")";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        Hero()->BlockTalk();
    }
    else if (msg.find("#unlock") == 0)
    {
        string cmd;
        float key = 1.0;
        istringstream is(msg);
        is >> cmd;
        char c;
        while (is >> c)
            key += (float)(unsigned char)c / 255.0;
        key = fabs(tan(key)) * 10000.0;
        int key2 = (int)key;
        //ostringstream os;
        //os << "Key generated " << key2;
        //MessageBox(0, os.str().c_str(), "Info", MB_OK);
        // 361
        //if ((key2 & 7) != 1)
        //{
        //    MessageBox(0, "Invalid unlock key detected :(", "Info", MB_OK);
        //}
        //else
        //{
            _secretCall(key2, (void*)this);
        //}
        Hero()->BlockTalk();
    }
    else if (msg.find("#fkey") == 0)
    {
        string cmd;
        int fkey;
        istringstream is(msg);
        is >> cmd >> fkey;
        FKey(fkey);
    }
    else if (msg == "#speed off")
    {
        char packet[] = {0x24, 0x00, 0x21, 0x27, 0x69, 0x69, 0x2A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00/*0x80*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *(int*)&packet[4] = Hero()->UniqueId();
        RecvPacket(packet, sizeof(packet));
    }
    else if (msg.find("#speed") == 0)
    {
        char packet[] = {0x24, 0x00, 0x21, 0x27, 0x69, 0x69, 0x2A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00/*0x80*/, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *(int*)&packet[4] = Hero()->UniqueId();
        RecvPacket(packet, sizeof(packet));
    }
    else if (msg.find("#dead") == 0)
    {
        char packet[] = {0x24, 0x00, 0x21, 0x27, 0x69, 0x69, 0x2A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *(int*)&packet[4] = Hero()->UniqueId();
        RecvPacket(packet, sizeof(packet));
    }
    else if (msg.find("#invis") == 0)
    {
        char packet[] = {0x24, 0x00, 0x21, 0x27, 0x69, 0x69, 0x2A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        for (int i = 0; i < NumMobs(); ++i)
        {
            Roles::CRole* mob = GetMob(i);
            *(int*)&packet[4]  = mob->UniqueId();
            *(int*)&packet[16] = 0x20 | 0x800;
            RecvPacket(packet, sizeof(packet));
            delete mob;
        }
    }
    else if (msg.find("#test") == 0)
    {
        char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00};
        SendPacket(packet, sizeof(packet));
    }
    else if (_unlocked)
    {
        if (msg.find("#portal") == 0)
        {
            string cmd;
            int num;
            istringstream is(msg);
            is >> cmd >> num;
            char packet[] = {0x24, 0x00, 0x1A, 0x27, 0xE6, 0xEA, 0x19, 0x00, 0xC3, 0x03, 0x2D, 0x02, 0x00, 0x00, 0x00, 0x00, 0x29, 0x2E, 0x11, 0x00, 0x55, 0x00, 0x07, 0x00, 0xC3, 0x03, 0x2E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
            int i = sizeof(packet) - 4;
            *(int*)&packet[i] = num;
            SendPacket(packet, sizeof(packet));
            Hero()->BlockTalk();
        }
    }
}

void CClient::_Unlock(CClient* _this)
{
    //MessageBox(0, "Congratualations, the unlock key is correct!", "Info", MB_OK);
    _this->_unlocked = true;
}

CClient::CClient(const char* baseDir):
    _addresses(baseDir),
    _hero(0),
    _updateHook(0),
    _itemListHook(&_addresses),
    _recvPacketHook(0),
    _diceKingHook(0),
    _meleeBot(0),
    _gotoBot(0),
    _looterBot(0),
    _xpSkillBot(0),
    _botJailBot(0),
    _potterBot(0),
    _attackSpammerBot(0),
    _followerBot(0),
    _disconnectorBot(0),
    _gamblerBot(0),
    _unlocked(false)
{
    _hero = new Roles::CHero(this, &_addresses);
    Hero()->OnTalk += new CEvent::T<CClient>(this, &CClient::_Command);
    _updateHook = new Hooks::CUpdate(this, &_addresses);
    _updateHook->Patch();
    _recvPacketHook = new Hooks::CRecvPacket(this, &_addresses);
    _recvPacketHook->Patch();
    _diceKingHook = new Hooks::CDiceKing(&_addresses);
    _diceKingHook->Patch();
    _itemListHook.Patch();
    _meleeBot = new Bots::CMelee(this);
    _gotoBot = new Bots::CGoto(this);
    _looterBot = new Bots::CLooter(this, baseDir);
    _xpSkillBot = new Bots::CXpSkill(this);
    _botJailBot = new Bots::CBotJail(this);
    _potterBot = new Bots::CPotter(this);
    _attackSpammerBot = new Bots::CAttackSpammer(this);
    _followerBot = new Bots::CFollower(this);
    _disconnectorBot = new Bots::CDisconnector(this);
    _gamblerBot = new Bots::CGambler(this, baseDir);
    // 361
    char code[20];
    int tmp = (int)_Unlock;
    code[0] = 0x68;
    code[1] = ((char*)&tmp)[0];
    code[2] = ((char*)&tmp)[1];
    code[3] = ((char*)&tmp)[2];
    code[4] = ((char*)&tmp)[3];
    code[5] = 0xC3;
    _secretCall.SetCode(361, code);
    // DiceKing test
    _diceKingHook->OnRole += new CEvent::T<CClient>(this, &CClient::_DiceKingRole);
}

CClient::~CClient()
{
    delete _hero;
    _recvPacketHook->Unpatch();
    _updateHook->Unpatch();
    _diceKingHook->Unpatch();
    _itemListHook.Unpatch();
    delete _recvPacketHook;
    delete _updateHook;
    delete _diceKingHook;
    delete _meleeBot;
    delete _gotoBot;
    delete _looterBot;
    delete _xpSkillBot;
    delete _botJailBot;
    delete _potterBot;
    delete _attackSpammerBot;
    delete _followerBot;
    delete _disconnectorBot;
    delete _gamblerBot;
}

int CClient::NumMobs()
{
    int n;
    __asm
    (
        "pushl %%ebx\n\t"
        "leal 0x10(%%ebx), %%ecx\n\t"
        "call *%%eax"
        : "=a"(n) : "a"(_addresses.NumMobsFn), "b"(_addresses.MobListEbx) :
    );
    return n;
}

Roles::CRole* CClient::GetMob(int i)
{
    int x;
    __asm
    (
        "pushl 0xC(%%ebx)\n\t"
        "pushl 0x8(%%ebx)\n\t"
        "pushl 0x4(%%ebx)\n\t"
        "pushl (%%ebx)\n\t"
        "pushl %%ecx\n\t"
        "leal 4(%%esp), %%ecx\n\t"
        "call *%%eax\n\t"
        "movl (%%eax), %%eax\n\t"
        "addl $0x10, %%esp"
        : "=a"(x) : "a"(_addresses.GetMobFn), "b"(_addresses.MobListEbx), "c"(i) :
    );
    return new Roles::CRole(&_addresses, x);
}

int CClient::ReadGround(int x, int y)
{
    /*
    int ret;
    __asm("pushal");
    __asm
    (
        "movl %%ecx, %%esi\n\t"
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : "=a"(ret) : "a"(_addresses.ReadGroundFn), "c"(_addresses.ReadGroundEcx), "b"(x), "edx"(y) :
    );
    //
    __asm
    (
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : "=a"(ret) : "a"(0x00520CA1), "b"(x), "d"(y), "c"(0x71CBA8)
    );
    //
    ret = ((char*)ret)[4];
    __asm("popal");
    return ret;
    */
    //int maxWidth   = *(int*)(_addresses.ReadGroundEcx + 0x34);
    int maxHeight  = *(int*)(_addresses.ReadGroundEcx + 0x30);
    int groundBase = *(int*)(_addresses.ReadGroundEcx + 0x54);
    int groundPtr  = ((y * maxHeight + x) << 4) + groundBase;
    return ((char*)groundPtr)[2];
}

void CClient::FreeGround(int* x, int* y)
{
    bool right = true;
    int size = 1;
    while (ReadGround(*x, *y) == 1)
    {
        if (right)
        {
            for (int i = 0; i < size; ++i)
            {
                ++*x;
                if (ReadGround(*x, *y) == 0)
                    break;
            }
            for (int i = 0; i < size; ++i)
            {
                ++*y;
                if (ReadGround(*x, *y) == 0)
                    break;
            }
        }
        else
        {
            for (int i = 0; i < size; ++i)
            {
                --*x;
                if (ReadGround(*x, *y) == 0)
                    break;
            }
            for (int i = 0; i < size; ++i)
            {
                --*y;
                if (ReadGround(*x, *y) == 0)
                    break;
            }
        }
        right = !right;
        ++size;
    }
}

void CClient::SendPacket(char* packet, int size)
{
    __asm
    (
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : : "a"(_addresses.SendPacketFn), "b"(packet), "c"(_addresses.SendPacketEcx), "d"(size) :
    );
}

/*
$+E      > 57               PUSH EDI
$+F      > FF75 E8          PUSH DWORD PTR SS:[EBP-18]
$+12     > E8 29070000      CALL Conquer.005765A7                    ; 1st Recv Process
$+17     > 8BF0             MOV ESI,EAX
$+19     > 59               POP ECX
$+1A     > 85F6             TEST ESI,ESI
$+1C     > 59               POP ECX
$+1D     > 8975 E0          MOV DWORD PTR SS:[EBP-20],ESI
$+20     > 0F84 42010000    JE Conquer.00575FCF
$+26     > 0FB75E 06        MOVZX EBX,WORD PTR DS:[ESI+6]
$+2A     > 895D BC          MOV DWORD PTR SS:[EBP-44],EBX
$+2D     > E8 34FDF9FF      CALL Conquer.00515BCD                    ; 2nd Recv Process
$+32     > 53               PUSH EBX
$+33     > 8BC8             MOV ECX,EAX
$+35     > E8 702E0500      CALL Conquer.005C8D11                    ; 3rd Recv Process
$+3A     > 8B06             MOV EAX,DWORD PTR DS:[ESI]
$+3C     > 6A 00            PUSH 0
$+3E     > 8BCE             MOV ECX,ESI
$+40     > C645 FC 07       MOV BYTE PTR SS:[EBP-4],7
$+44     > FF50 10          CALL DWORD PTR DS:[EAX+10]               ; 4th Recv Process
*/

void CClient::RecvPacket(char* packet, int size)
{
    int esi, ecx;
    __asm
    (
        "pushl %%ebx\n\t"
        "pushl %%ecx\n\t"
        "call *%%eax\n\t"
        "popl %%ecx\n\t"
        "popl %%ecx"
        : "=a"(esi) : "a"(_addresses.RecvPacketHook), "c"(packet), "b"(size) :
    );
    if (esi != 0)
    {
        __asm("call *%%eax" : "=a"(ecx) : "a"(_addresses.ProcessRecvFn2) : );
        __asm
        (
            "pushl %%ebx\n\t"
            "call *%%eax\n\t"
            "mov (%%esi), %%eax\n\t"
            "pushl $0\n\t"
            "movl %%esi, %%ecx\n\t"
            "call *0x10(%%eax)"
            : : "a"(_addresses.ProcessRecvFn3), "b"(*(short*)(esi+6)), "c"(ecx), "S"(esi) :
        );
    }
}

void CClient::FKey(int num)
{
    __asm
    (
        "pushl %%edi\n\t"
        "pushl %%edx\n\t"
        "pushl %%ecx\n\t"
        "movl (%%ecx), %%ecx\n\t"
        //"addl $0xB268C, %%ecx\n\t"
        "addl %%edi, %%ecx\n\t"
        "pushl %%ebx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax\n\t"
        "popl %%ebx\n\t"
        "incl %%ebx\n\t"
        "popl %%ecx\n\t"
        "popl %%edx\n\t"
        "addl $4, %%ecx\n\t"
        "movl (%%ecx), %%ecx\n\t"
        "shll $8, %%eax\n\t"
        "orl %%ebx, %%eax\n\t"
        "pushl %%eax\n\t"
        "call *%%edx\n\t"
        "popl %%edi"
        : : "a"(_addresses.GetFKeyItemFn), "d"(_addresses.ExecuteFKeyItemFn), "c"(_addresses.FKeyItemBase), "b"(num-1), "D"(_addresses.FKeyItemOffset) :
    );
}

void CClient::Disconnect()
{
    *(int*)(_addresses.DisconnectEcx + 0x14) = 0;
}
