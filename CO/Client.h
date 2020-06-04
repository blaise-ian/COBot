#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "../SecretCall.h"
#include "../Event.h"
#include "Addresses.h"
#include "Roles/Hero.h"
#include "Roles/Role.h"
#include "Hooks/Update.h"
#include "Hooks/ItemList.h"
#include "Hooks/RecvPacket.h"
#include "Hooks/DiceKing.h"
#include "Bots/Melee.h"
#include "Bots/Goto.h"
#include "Bots/Looter.h"
#include "Bots/XpSkill.h"
#include "Bots/Botjail.h"
#include "Bots/Potter.h"
#include "Bots/AttackSpammer.h"
#include "Bots/Follower.h"
#include "Bots/Disconnector.h"
#include "Bots/Gambler.h"

namespace CO
{
    namespace Roles
    {
        class CHero;
    }

    namespace Hooks
    {
        class CUpdate;
        class CRecvPacket;
        class CDiceKing;
    }

    namespace Bots
    {
        class CMelee;
        class CGoto;
        class CLooter;
        class CXpSkill;
        class CBotJail;
        class CPotter;
        class CAttackSpammer;
        class CFollower;
        class CDisconnector;
        class CGambler;
    }

    class CClient
    {
    private:
        CAddresses          _addresses;
        Roles::CHero*       _hero;
        Hooks::CUpdate*     _updateHook;
        Hooks::CItemList    _itemListHook;
        Hooks::CRecvPacket* _recvPacketHook;
        Hooks::CDiceKing*   _diceKingHook;
        Bots::CMelee*       _meleeBot;
        Bots::CGoto*        _gotoBot;
        Bots::CLooter*      _looterBot;
        Bots::CXpSkill*     _xpSkillBot;
        Bots::CBotJail*     _botJailBot;
        Bots::CPotter*      _potterBot;
        Bots::CAttackSpammer* _attackSpammerBot;
        Bots::CFollower*    _followerBot;
        Bots::CDisconnector* _disconnectorBot;
        Bots::CGambler*     _gamblerBot;
        CSecretCall         _secretCall;
        bool                _unlocked;

        void _DiceKingRole(void* sender, void** params);
        void _Command(void* sender, void** params);
        static void _Unlock(CClient* _this);
    public:
        CClient(const char* baseDir);
        ~CClient();

        CEvent OnUpdate;
        CEvent OnRecvPacket;
        CEvent OnDiceKingRole;

        bool IsUnlocked() const { return _unlocked; }

        Bots::CLooter* LooterBot() const { return _looterBot; }
        Bots::CXpSkill* XpSkillBot() const { return _xpSkillBot; }
        Bots::CGoto* GotoBot() const { return _gotoBot; }

        Roles::CHero* Hero() { return _hero; }
        int NumMobs();
        Roles::CRole* GetMob(int i);

        int NumGroundItems() { return _itemListHook.Count(); }
        void GetGroundItem(int i, int* dropId, int* itemId, int* x, int* y)
        {
            _itemListHook.Get(i, dropId, itemId, x, y);
        }

        int ReadGround(int x, int y);
        void FreeGround(int* x, int* y);
        void SendPacket(char* packet, int size);
        void RecvPacket(char* packet, int size);
        void FKey(int num);
        void Disconnect();
    };
}

#endif // CLIENT_H_INCLUDED
