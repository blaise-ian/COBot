#ifndef ADDRESSES_H_INCLUDED
#define ADDRESSES_H_INCLUDED

#include <string>

namespace CO
{
    class CAddresses
    {
    private:
        std::string     _baseDir;

        void _DetectAddresses();
    public:
        CAddresses(const char* baseDir);
        //CAddresses();
        ~CAddresses();

        int HeroBasePtr;
        int JumpFn;
        int MeleeFn;
        int MobListEbx;
        int NumMobsFn;
        int GetMobFn;
        int TalkFn;
        int UpdateHookAddr;
        int ReadGroundEcx;
        int ReadGroundFn;
        int GroundItemListHook;
        int LootFn;
        int SendPacketEcx;
        int SendPacketFn;
        int RecvPacketHook;
        int RefreshCoordsFn;
        int NumInvItemsFn;
        int GetInvItemFn;
        int DropItemFn;
        int XpSkillFn;
        int XpSkillInfoFn;
        int IsXpSkillReadyFn;
        int GetHpMpFn;
        int FKeyItemBase;
        int FKeyItemOffset;
        int GetFKeyItemFn;
        int ExecuteFKeyItemFn;
        int ProcessRecvFn2;
        int ProcessRecvFn3;
        int DisconnectEcx;
        int DiceKingHook;
    };
}

#endif // ADDRESSES_H_INCLUDED
