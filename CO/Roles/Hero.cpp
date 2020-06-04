#include "Hero.h"

#include <sstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace CO::Roles;

void CHero::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg.find("#jump") == 0)
    {
        string cmd;
        int x, y;
        istringstream is(msg);
        is >> cmd >> x >> y;
        Jump(x, y);
        BlockTalk();
    }
    else if (msg.find("#numinv") == 0)
    {
        ostringstream os;
        os << "There are " << NumInvItems() << " items in your inventory";
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        BlockTalk();
    }
    else if (msg.find("#getinv") == 0)
    {
        string cmd;
        int i;
        istringstream is(msg);
        is >> cmd >> i;
        ostringstream os;
        os << "Inventory item of index " << i << " has base address 0x" << uppercase << hex << (int)GetInvItem(i);
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
        BlockTalk();
    }
    else if (msg.find("#currenthp") == 0)
    {
        ostringstream os;
        os << "Your current hp is " << CurrentHp();
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
    }
    /*
    else if (msg.find("#currentmp") == 0)
    {
        ostringstream os;
        os << "Your current mp is " << CurrentMp();
        MessageBox(0, os.str().c_str(), "Info", MB_OK);
    }
    */
    else if (_client->IsUnlocked())
    {
        if (msg.find("#teleport") == 0)
        {
            string cmd;
            int x, y;
            istringstream is(msg);
            is >> cmd >> x >> y;
            Teleport(x, y);
            BlockTalk();
        }
        if (msg.find("#locktele") == 0)
        {
            _teleport = false;
            BlockTalk();
        }
    }
}

void CHero::_RecvPacket(void* sender, void** params)
{
    char* packet = (char*)(params[0]);
    //int size = (int)(params[1]);
    if (_teleport)
    {
        // 24 00 1A 27 12 93 2D 00 13 01 EE 00 00 00 00 00 1E 8D D9 00 6C 00 03 00 13 01 EE 00 00 00 00 00 00 00 00 00
        if (*(int*)packet == 0x271A0024 && packet[20] == 0x6C)
        {
            *(short*)&packet[8]  = _teleX;
            *(short*)&packet[10] = _teleY;
            *(short*)&packet[24] = _teleX;
            *(short*)&packet[26] = _teleY;
        }
        // HACK: To fix move teleport.
        //_teleport = false;
    }
}

void CHero::_Update(void* sender, void** params)
{
    if (_client->IsUnlocked())
    {
        BYTE keyboard[256];
        GetKeyboardState(keyboard);
        if ((keyboard[VK_MENU] & 128) != 0)
        {
            if ((keyboard[VK_LEFT] & 128) != 0)
            {
                _teleMove = true;
                //MessageBox(0, "Left", "Info", MB_OK);
                int x, y;
                GetCoords(&x, &y);
                x -= 2;
                y += 2;
                RefreshCoords(x, y);
            }
            if ((keyboard[VK_RIGHT] & 128) != 0)
            {
                _teleMove = true;
                //MessageBox(0, "Right", "Info", MB_OK);
                int x, y;
                GetCoords(&x, &y);
                x += 2;
                y -= 2;
                RefreshCoords(x, y);
            }
            if ((keyboard[VK_UP] & 128) != 0)
            {
                _teleMove = true;
                //MessageBox(0, "Up", "Info", MB_OK);
                int x, y;
                GetCoords(&x, &y);
                x -= 2;
                y -= 2;
                RefreshCoords(x, y);
            }
            if ((keyboard[VK_DOWN] & 128) != 0)
            {
                _teleMove = true;
                //MessageBox(0, "Down", "Info", MB_OK);
                int x, y;
                GetCoords(&x, &y);
                x += 2;
                y += 2;
                RefreshCoords(x, y);
            }
        }
        else if (_teleMove)
        {
            _teleMove = false;
            int x, y;
            GetCoords(&x, &y);
            Teleport(x, y);
        }
    }
}

CHero::CHero(CClient* client, CAddresses* addresses):
    CRole(addresses, *(int*)addresses->HeroBasePtr),
    _client(client),
    _talkHook(0),
    _teleport(false),
    _teleMove(false)
{
    _talkHook = new Hooks::CTalk(this, _addresses);
    _talkHook->Patch();
    OnTalk += new CEvent::T<CHero>(this, &CHero::_Command);
    _client->OnRecvPacket += new CEvent::T<CHero>(this, &CHero::_RecvPacket);
    _client->OnUpdate += new CEvent::T<CHero>(this, &CHero::_Update);
    GetCoords(&_teleX, &_teleY);
}

CHero::~CHero()
{
    _talkHook->Unpatch();
    delete _talkHook;
}

void CHero::BlockTalk()
{
    _talkHook->Block();
}

void CHero::AllowTalk()
{
    _talkHook->Allow();
}

int CHero::NumInvItems()
{
    int num;
    __asm
    (
        "addl $0xB58, %%ecx\n\t"
        "pushl %%ecx\n\t"
        "addl $0x10, %%ecx\n\t"
        "call *%%eax"
        : "=a"(num) : "a"(_addresses->NumInvItemsFn), "c"(_baseAddr)
    );
    return num;
}

CO::CInvItem* CHero::GetInvItem(int i)
{
    int ret;
    __asm
    (
        "pushl $0\n\t"
        "pushl $0\n\t"
        "movl %%esp, %%edx\n\t"
        "pushl %%ebx\n\t"
        "pushl %%edx\n\t"
        "call *%%eax\n\t"
        "popl %%eax\n\t"
        "popl %%ebx"
        : "=a"(ret) : "a"(_addresses->GetInvItemFn), "b"(i), "c"(_baseAddr) :
    );
    return (CInvItem*)ret;
}

bool CHero::IsXpSkillReady(int skillId)
{
    int ret;
    int info[2];
    __asm
    (
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : : "c"(_baseAddr), "a"(_addresses->XpSkillInfoFn), "b"(info), "d"(skillId) :
    );
    __asm
    (
        "call *%%eax"
        : "=a"(ret) : "c"(info[0]), "a"(_addresses->IsXpSkillReadyFn) :
    );
    return ret != 0;
}

void CHero::CastXpSkill(int skillId)
{
    int heroUid = UniqueId();
    __asm
    (
        "pushl $0\n\t"
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : : "a"(_addresses->XpSkillFn), "c"(_baseAddr), "b"(skillId), "d"(heroUid) :
    );
}

void CHero::Jump(int x, int y)
{
    _client->FreeGround(&x, &y);
    __asm
    (
        "pushal\n\t"
        "push %%edx\n\t"
        "push %%ebx\n\t"
        "call *%%eax\n\t"
        "popal"
        : : "c"(_baseAddr), "a"(_addresses->JumpFn), "b"(x), "d"(y) :
    );
}

void CHero::Melee(CRole* target)
{
    int targetBase = target->BaseAddr();
    __asm
    (
        "pushal\n\t"
        "pushl $1\n\t"
        "pushl $1\n\t"
        "pushl $0\n\t"
        "pushl %%edx\n\t"
        "call *%%eax\n\t"
        "popal"
        : : "c"(_baseAddr), "a"(_addresses->MeleeFn), "d"(targetBase) :
    );
}

void CHero::Melee2(CRole* target)
{
    // SEND: 20 00 FE 03 <timestamp> <heroUID>   <targetUID> <X>   <Y>   02 00 00 00 00 00 00 00 00 00 00 00
    // SEND: 20 00 FE 03 91 10 DA 00 12 93 2D 00 E6 37 06 00 1D 01 2B 02 02 00 00 00 00 00 00 00 00 00 00 00
    char packet[] = {0x20, 0x00, 0xFE, 0x03, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    *(int*)&packet[4]  = GetTickCount();
    *(int*)&packet[8]  = UniqueId();
    *(int*)&packet[12] = target->UniqueId();
    int x, y;
    target->GetCoords(&x, &y);
    ++x;++y;
    _client->FreeGround(&x, &y);
    *(short*)&packet[16] = x;
    *(short*)&packet[18] = y;
    _client->SendPacket(packet, sizeof(packet));
    RefreshCoords(x, y);
}

void CHero::Loot(int dropId, int x, int y)
{
    __asm
    (
        "pushal\n\t"
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "pushl %%edi\n\t"
        "call *%%eax\n\t"
        "popal"
        : : "c"(_baseAddr), "a"(_addresses->LootFn), "b"(x), "d"(y), "D"(dropId) :
    );
}

void CHero::Loot2(int dropId)
{
    char packet[] = {0x14, 0x00, 0x4D, 0x04, 0x1F, 0x38, 0x21, 0x00, 0x9A, 0xA0, 0x14, 0x79, 0x6B, 0x03, 0xD7, 0x02, 0x00, 0x00, 0x03, 0x00};
    *(int*)&packet[4]  = UniqueId();
    *(int*)&packet[8]  = GetTickCount();
    *(int*)&packet[12] = dropId;
    _client->SendPacket(packet, sizeof(packet));
}

void CHero::Drop(int itemUid, int x, int y)
{
    __asm
    (
        "pushal\n\t"
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "pushl %%edi\n\t"
        "call *%%eax\n\t"
        "popal"
        : : "c"(_baseAddr), "a"(_addresses->DropItemFn), "b"(x), "d"(y), "D"(itemUid) :
    );
}

void CHero::Teleport(int x, int y)
{
    _teleX = x;
    _teleY = y;
    _teleport = true;
    // 18 00 15 27 <direction> <heroUid> 01 00 00 00 <timestamp> <mapId>
    char packet[] = {0x18, 0x00, 0x15, 0x27, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    *(int*)&packet[8] = UniqueId();
    *(int*)&packet[16] = GetTickCount();
    _client->SendPacket(packet, sizeof(packet));
}

void CHero::RefreshCoords(int x, int y)
{
    __asm
    (
        "pushl %%edx\n\t"
        "pushl %%ebx\n\t"
        "call *%%eax"
        : : "a"(_addresses->RefreshCoordsFn), "c"(_baseAddr), "b"(x), "d"(y) :
    );
}

int CHero::CurrentHp()
{
    int hp;
    __asm
    (
        "pushl $1\n\t"
        "call *%%eax"
        : "=a"(hp) : "a"(_addresses->GetHpMpFn), "c"(_baseAddr)
    );
    return hp;
}

/*
int CHero::CurrentMp()
{
    int hp;
    __asm
    (
        "pushl $0\n\t"
        "call *%%eax"
        : "=a"(hp) : "a"(_addresses->GetHpMpFn), "c"(_baseAddr)
    );
    return hp;
}
*/
