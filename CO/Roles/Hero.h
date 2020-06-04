#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "../../Event.h"
#include "../Addresses.h"
#include "../Client.h"
#include "../InvItem.h"
#include "../Hooks/Talk.h"
#include "Role.h"

namespace CO {
    class CClient;
    namespace Hooks {
        class CTalk;
    }
namespace Roles {

    class CHero: public CRole
    {
    private:
        CClient*      _client;
        Hooks::CTalk* _talkHook;
        bool          _teleport;
        int           _teleX;
        int           _teleY;
        bool          _teleMove;

        void _Command(void* sender, void** params);
        void _RecvPacket(void* sender, void** params);
        void _Update(void* sender, void** params);
    public:
        CHero(CClient* client, CAddresses* addresses);
        ~CHero();

        CEvent OnTalk;

        void BlockTalk();
        void AllowTalk();

        int NumInvItems();
        CInvItem* GetInvItem(int i);

        bool IsXpSkillReady(int skillId);
        void CastXpSkill(int skillId);
        void Jump(int x, int y);
        void Melee(CRole* target);
        void Melee2(CRole* target);
        void Loot(int dropId, int x, int y);
        void Loot2(int dropId);
        void Drop(int itemUid, int x, int y);
        void Teleport(int x, int y);
        void RefreshCoords(int x, int y);
        int CurrentHp();
        //int CurrentMp();
    };

}
}
#endif // HERO_H_INCLUDED
