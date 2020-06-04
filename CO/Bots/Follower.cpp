#include "Follower.h"

#include <sstream>

using namespace CO::Bots;
using namespace std;

void CFollower::_Update(void* sender, void** params)
{
    if (_running)
    {
        if (GetTickCount() - _lastTime < 300)
            return;
        _lastTime = GetTickCount();
        Roles::CRole* targetMob = 0;
        for (int i = 0; i < _client->NumMobs(); ++i)
        {
            Roles::CRole* mob = _client->GetMob(i);
            if (_target == mob->Name())
            {
                targetMob = mob;
                break;
            }
            delete mob;
        }
        if (targetMob != 0)
        {
            int mx, my;
            targetMob->GetCoords(&mx, &my);
            ++mx; ++my;
            _client->FreeGround(&mx, &my);
            //_client->GotoBot()->Goto(mx, my);
            _client->Hero()->Jump(mx, my);
            delete targetMob;
        }
    }
}

void CFollower::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#follow off")
    {
        _running = false;
    }
    else if (msg.find("#follow") == 0)
    {
        istringstream is(msg);
        string cmd;
        is >> cmd >> _target;
        _lastTime = GetTickCount();
        _running = true;
    }
}

CFollower::CFollower(CClient* client): _client(client), _running(false)
{
    _client->OnUpdate += new CEvent::T<CFollower>(this, &CFollower::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CFollower>(this, &CFollower::_Command);
}
