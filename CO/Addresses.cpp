#include "Addresses.h"
#include "../MemorySearcher.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace CO;

void CAddresses::_DetectAddresses()
{
    CMemorySearcher memorySearcher(0x400000, 0x700000);
    HeroBasePtr = memorySearcher.ReadInt32(memorySearcher.Find("\x55\x8B\xEC\x51\x8B\x45\x08\x8B\x0D", 9) + 9);
    JumpFn = memorySearcher.Find("\xE9\x9B\x00\x00\x00\xF6\x45\x08\x08\x74\x14\xFF\x75\xE0\xFF\x75\xDC\xE8", 18) + 25;
    JumpFn += 4 + memorySearcher.ReadInt32(JumpFn);
    MeleeFn = memorySearcher.Find("\x8D\x45\xEC\x8B\xCC\x89\x65\x08\x50\xE8", 10) + 25;
    MeleeFn += 4 + memorySearcher.ReadInt32(MeleeFn);
    MobListEbx = memorySearcher.ReadInt32(memorySearcher.Find("\x89\x85\xD0\xFA\xFF\xFF\x8B\x85\x0C\xFA\xFF\xFF\x89\x85\xD4\xFA\xFF\xFF\xFF\xB5\xD4\xFA\xFF\xFF\xFF\xB5\xD0\xFA\xFF\xFF\xB9", 31) + 31) + 0xC;
    NumMobsFn = memorySearcher.Find("\x8D\x59\x0C\x56\x57\x89\x4D\xEC\x53\x8D\x4B\x10\xE8", 13) + 13;
    NumMobsFn += 4 + memorySearcher.ReadInt32(NumMobsFn);
    GetMobFn = memorySearcher.Find("\x8D\x7D\xC8\xA5\xA5\xFF\x75\xF0\x8D\x4D\xC8\xA5\xA5\xE8", 14) + 14;
    GetMobFn += 4 + memorySearcher.ReadInt32(GetMobFn);
    TalkFn = memorySearcher.Find("\x8D\x85\xF4\xFE\xFF\xFF\x8B\xCB\xFF\x75\x20\xFF\x75\x1C\xFF\x75\x18\xFF\x75\x14\xFF\x75\x10\x50\xE8", 25) + 25;
    TalkFn += 4 + memorySearcher.ReadInt32(TalkFn);
    UpdateHookAddr = memorySearcher.Find("\x8D\x7D\xC8\xA5\xA5\xFF\x75\xF0\x8D\x4D\xC8\xA5\xA5\xE8", 14) - 0x2D;
    int groundBase = memorySearcher.Find("\x8B\xD0\x55\x53\x8B\xCE\x89\x54\x24\x18", 10);
    ReadGroundEcx = memorySearcher.ReadInt32(groundBase - 0xF);
    ReadGroundFn = memorySearcher.ReadInt32(groundBase + 0xB) + groundBase + 0xF;
    GroundItemListHook = memorySearcher.Find("\x51\x51\x8B\x41\x08\x83\xC1\x04\x53\x56\x2B\x01\x57\x89\x4D\xEC\xC1\xF8\x02\x48\x85\xC0", 22) + 5;
    LootFn = memorySearcher.Find("\x5F\x5E\xEB\x18\xFF\x74\x24\x0C\xFF\x74\x24\x0C\xFF\x74\x24\x0C", 0x10) + 0x18;
    LootFn += 4 + memorySearcher.ReadInt32(LootFn);
    int sendPacketBase = memorySearcher.Find("\x0F\xB7\x4E\x04\x8D\x46\x04\x51\x50", 9);
    SendPacketEcx = memorySearcher.ReadInt32(sendPacketBase + 0xA);
    SendPacketFn = memorySearcher.ReadInt32(sendPacketBase + 0xF) + sendPacketBase + 0x13;
    int recvPacketBase = memorySearcher.Find("\x3B\x7D\xEC\x59\x59\x89\x7D\xD8\x7F\xBD\x85\xFF\x76\xB9\x57\xFF\x75\xE8", 0x12);
    RecvPacketHook = recvPacketBase + 0x13;
    RecvPacketHook += 4 + memorySearcher.ReadInt32(RecvPacketHook);
    RefreshCoordsFn = memorySearcher.Find("\x48\xF7\xD8\x1B\xC0\xF7\xD8\x5E\xC2\x08\x00\x53\x56\x8B\xF1\x57\xFF\x74\x24\x10\x8D\x9E\xD0\x02\x00\x00\x8B\xCB", 0x1C) + 0xB;
    NumInvItemsFn = memorySearcher.Find("\x05\x58\x0B\x00\x00\x50\x8D\x48\x10", 0x09) + 0xA;
    NumInvItemsFn += 4 + memorySearcher.ReadInt32(NumInvItemsFn);
    GetInvItemFn = memorySearcher.Find("\x81\xEC\x08\x01\x00\x00\x53\x56\x57\x8D\x45\xEC\xFF\x75\x08\x8B\xF1\x50", 0x12) + 0x1A;
    GetInvItemFn += 4 + memorySearcher.ReadInt32(GetInvItemFn);
    DropItemFn = memorySearcher.Find("\x8B\x4D\xF0\x8B\xD8\xF7\xDB\x1A\xDB\x80\x65\xFC\x00\xFE\xC3\x3B\xCF", 0x11) - 0x4;
    DropItemFn += 4 + memorySearcher.ReadInt32(DropItemFn);
    int xpSkillBase = memorySearcher.Find("\x83\xEC\x6C\x8B\x45\x08\x53\x0F\xB6\xD0\xC1\xF8\x08\x56\x8B\xF1\x89\x55\xF0\x25\xFF\xFF\xFF\x00\x4A\x57", 0x1A);
    XpSkillFn = xpSkillBase + 0xA4;
    XpSkillFn += 4 + memorySearcher.ReadInt32(XpSkillFn);
    XpSkillInfoFn = xpSkillBase + 0x5D;
    XpSkillInfoFn += 4 + memorySearcher.ReadInt32(XpSkillInfoFn);
    IsXpSkillReadyFn = xpSkillBase + 0x70;
    IsXpSkillReadyFn += 4 + memorySearcher.ReadInt32(IsXpSkillReadyFn);
    GetHpMpFn = memorySearcher.Find("\xC3\x33\xC0\xC3\x8B\x89\xB0\x08\x00\x00\x85\xC9\x74\x0B\xFF\x74\x24\x04", 0x12) + 0x4;
    int fkeyBase = memorySearcher.Find("\x6A\x09\xEB\x12\x6A\x08\xEB\x0E\x6A\x07\xEB\x0A\x6A\x06\xEB\x06\x6A\x05\xEB\x02\x6A\x04\x5E\x6A\x01\x8D\x7E\x01\x58\x50", 0x1E);
    FKeyItemBase = memorySearcher.ReadInt32(fkeyBase + 0x1F);
    FKeyItemOffset = memorySearcher.ReadInt32(fkeyBase + 0x26);
    GetFKeyItemFn = memorySearcher.ReadInt32(fkeyBase + 0x44) + fkeyBase + 0x48;
    ExecuteFKeyItemFn = memorySearcher.ReadInt32(fkeyBase + 0x55) + fkeyBase + 0x59;
    ProcessRecvFn2 = memorySearcher.ReadInt32(recvPacketBase + 0x2E) + recvPacketBase + 0x32;
    ProcessRecvFn3 = memorySearcher.ReadInt32(recvPacketBase + 0x36) + recvPacketBase + 0x3A;
    DisconnectEcx = memorySearcher.ReadInt32(memorySearcher.Find("\x83\x65\xFC\x00\x53\x8B\xD9\x56\x57\x89\x5D\xDC\x8B\x4B\x14\x89\x65\xF0\x85\xC9", 0x14) - 0x1E);
    DiceKingHook = memorySearcher.Find("\x53\x0F\xB6\x4A\x12\x51\x0F\xB6\x4A\x11\x51\x0F\xB6\x4A\x10\x8B\x10\x51", 0x12) + 0x12;
    /*
    char buff[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, buff);
    SetCurrentDirectory(_baseDir.c_str());
    string filepath = "COAddresses-melee-dll.dat";
    ofstream os(filepath.c_str());
    os << HeroBasePtr << " " << JumpFn << " " << MeleeFn << " " << MobListEbx << " " << NumMobsFn << " " << GetMobFn << endl;
    os << TalkFn << " " << UpdateHookAddr << " " << ReadGroundEcx << " " << ReadGroundFn << " " << GroundItemListHook << " " << LootFn << endl;
    os << SendPacketEcx << " " << SendPacketFn << " " << RecvPacketHook << " " << RefreshCoordsFn << " " << NumInvItemsFn << " " << GetInvItemFn << endl;
    os << DropItemFn << " " << XpSkillFn << " " << XpSkillInfoFn << " " << IsXpSkillReadyFn << " " << GetHpMpFn << " " << FKeyItemBase << endl;
    os << FKeyItemOffset << " " << GetFKeyItemFn << " " << ExecuteFKeyItemFn << " " << ProcessRecvFn2 << " " << ProcessRecvFn3 << " " << DisconnectEcx;
    SetCurrentDirectory(buff);
    */
}

CAddresses::CAddresses(const char* baseDir):
    _baseDir(baseDir),
    HeroBasePtr(0),
    JumpFn(0),
    MeleeFn(0),
    MobListEbx(0),
    NumMobsFn(0),
    GetMobFn(0),
    TalkFn(0),
    UpdateHookAddr(0),
    ReadGroundEcx(0),
    ReadGroundFn(0),
    GroundItemListHook(0),
    LootFn(0),
    SendPacketEcx(0),
    SendPacketFn(0),
    RecvPacketHook(0),
    RefreshCoordsFn(0),
    NumInvItemsFn(0),
    GetInvItemFn(0),
    DropItemFn(0),
    XpSkillFn(0),
    XpSkillInfoFn(0),
    IsXpSkillReadyFn(0),
    GetHpMpFn(0),
    FKeyItemBase(0),
    FKeyItemOffset(0),
    GetFKeyItemFn(0),
    ExecuteFKeyItemFn(0),
    ProcessRecvFn2(0),
    ProcessRecvFn3(0),
    DisconnectEcx(0),
    DiceKingHook(0)
{
    /*
    char buff[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, buff);
    SetCurrentDirectory(_baseDir.c_str());
    string filepath = "COAddresses-melee-dll.dat";
    ifstream is(filepath.c_str());
    SetCurrentDirectory(buff);
    if (is)
    {
        is >> HeroBasePtr >> JumpFn >> MeleeFn >> MobListEbx >> NumMobsFn >> GetMobFn;
        is >> TalkFn >> UpdateHookAddr >> ReadGroundEcx >> ReadGroundFn >> GroundItemListHook >> LootFn;
        is >> SendPacketEcx >> SendPacketFn >> RecvPacketHook >> RefreshCoordsFn >> NumInvItemsFn >> GetInvItemFn;
        is >> DropItemFn >> XpSkillFn >> XpSkillInfoFn >> IsXpSkillReadyFn >> GetHpMpFn >> FKeyItemBase;
        is >> FKeyItemOffset >> GetFKeyItemFn >> ExecuteFKeyItemFn >> ProcessRecvFn2 >> ProcessRecvFn3 >> DisconnectEcx;
    }
    else
        _DetectAddresses();
    */
    _DetectAddresses();
}

CAddresses::~CAddresses()
{
}
