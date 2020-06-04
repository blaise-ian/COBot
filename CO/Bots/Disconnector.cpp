#include "Disconnector.h"

#include <string>
#include <sstream>
using namespace CO::Bots;
using namespace std;

void CDisconnector::_Update(void* sender, void** params)
{
    if (_running)
    {
        for (int i = 0; i < _client->NumMobs(); ++i)
        {
            Roles::CRole* mob = _client->GetMob(i);
            bool dc = false;
            if (_dcOnGm)
            {
                string name = mob->Name();
                if (name.find("[GM]") >= 0 || name.find("[PM]") >= 0)
                    dc = true;
            }
            int aniFlag = mob->AnimationFlag();
            if (_dcOnBlack && (aniFlag & 0x8000) != 0)
                dc = true;
            if (_dcOnRed && (aniFlag & 0x4000) != 0)
                dc = true;
            if (_dcOnBlue && (aniFlag & 0x1) != 0)
                dc = true;
            delete mob;
            if (dc)
            {
                _client->Disconnect();
                _running = false;
                break;
            }
        }
    }
}

void CDisconnector::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#dc off")
    {
        _running = false;
    }
    else if (msg.find("#dc") == 0)
    {
        string cmd;
        istringstream is(msg);
        is >> cmd;
        _dcOnBlack = false;
        _dcOnRed = false;
        _dcOnBlue = false;
        _dcOnGm = false;
        if (!(is >> cmd))
        {
            _client->Disconnect();
            _running = false;
        }
        else
        {
            do
            {
                if (cmd == "black")
                    _dcOnBlack = true;
                else if (cmd == "red")
                    _dcOnRed = true;
                else if (cmd == "blue")
                    _dcOnBlue = true;
                else if (cmd == "gm")
                    _dcOnGm = true;
            } while (is >> cmd);
            _running = true;
        }
    }
}

CDisconnector::CDisconnector(CClient* client):
    _client(client),
    _running(false),
    _dcOnBlack(false),
    _dcOnRed(false),
    _dcOnBlue(false),
    _dcOnGm(false)
{
    _client->OnUpdate += new CEvent::T<CDisconnector>(this, &CDisconnector::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CDisconnector>(this, &CDisconnector::_Command);
}
