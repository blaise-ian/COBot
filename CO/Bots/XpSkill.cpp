#include "XpSkill.h"

#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
using namespace CO::Bots;

void CXpSkill::_Update(void* sender, void** params)
{
    if (_running)
    {
        //if (GetTickCount() - _fatalStrikeTime >= 60000)
        //    IsFatalStrikeOn = false;
        if (GetTickCount() - _lastTime < 1000)
            return;
        _lastTime = GetTickCount();
        if (_client->Hero()->IsXpSkillReady(_skillId))
        {
            _client->Hero()->CastXpSkill(_skillId);
            //if (_skillId == 6011)
            //{
            //    IsFatalStrikeOn = true;
            //    _fatalStrikeTime = GetTickCount();
            //}
        }
    }
}

void CXpSkill::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#autoxp off")
    {
        _running = false;
        IsFatalStrikeOn = false;
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#autoxp") == 0)
    {
        string cmd;
        istringstream is(msg);
        is >> cmd >> _skillId;
        _running = true;
        _lastTime = GetTickCount();
        _client->Hero()->BlockTalk();
    }
}

void CXpSkill::_RecvPacket(void* sender, void** params)
{
    char* packet = (char*)(params[0]);
    //int size = (int)(params[1]);
    //ostringstream os;
    //os << "RECV: ";
    //for (int i = 0; i < (int)(params[1]); ++i)
    //{
    //    os << hex << uppercase << setw(2) << setfill('0') << right << (int)packet[i] << " ";
    //}
    //MessageBox(0, os.str().c_str(), "Info", MB_OK);
    // Deactivate FS:
    // RECV: 24 00 21 27 50 F0 1D 00 01 00 00 00 19 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    // Activate FS:
    // RECV: 24 00 21 27 12 93 2D 00 01 00 00 00 19 00 00 00 00 00 00 00 00 80 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    if (*(int*)packet == 0x27210024 &&
        *(int*)&packet[8] == 0x01 &&
        *(int*)&packet[12] == 0x19 &&
        *(int*)&packet[4] == _client->Hero()->UniqueId())
    {
        /*
        if (*(int*)&packet[21] == 0x80)
            IsFatalStrikeOn = true;
        else
            IsFatalStrikeOn = false;
        */
        if (*(int*)&packet[20] & 0x8000)
            IsFatalStrikeOn = true;
        else
            IsFatalStrikeOn = false;
        if (*(int*)&packet[16] & 0x40000)
            *(int*)&packet[16] |= 0x800000;
        //MessageBox(0, "Status changed!", "Info", MB_OK);
        //*(int*)&packet[16] |= 0x800000;
        //*(int*)&packet[16] &= ~0x2; // turns off poison.
        //*(int*)&packet[16] |= 0x80000000; // on blessing
    }
}

CXpSkill::CXpSkill(CClient* client):
    _client(client),
    _running(false),
    IsFatalStrikeOn(false)
{
    _client->OnUpdate += new CEvent::T<CXpSkill>(this, &CXpSkill::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CXpSkill>(this, &CXpSkill::_Command);
    _client->OnRecvPacket += new CEvent::T<CXpSkill>(this, &CXpSkill::_RecvPacket);
}
