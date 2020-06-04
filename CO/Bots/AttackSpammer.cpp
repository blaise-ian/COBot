#include "AttackSpammer.h"
#include <windows.h>

#include <sstream>
using namespace CO::Bots;
using namespace std;

void CAttackSpammer::_Update(void* sender, void** params)
{
    if (_client->IsUnlocked())
    {
        if (GetTickCount() - _lastTime < 20)
            return;
    }
    else
    {
        if (GetTickCount() - _lastTime < 200)
            return;
    }
    _lastTime = GetTickCount();
    BYTE keys[256];
    GetKeyboardState(keys);
    if ((keys[VK_MENU] & 0x80) != 0 && (keys[(int)'A'] & 0x80) != 0)
    {
        if (_first)
        {
            _client->Hero()->GetCoords(&_px, &_py);
            _first = false;
        }
        /*
        int px, py;
        _client->Hero()->GetCoords(&px, &py);
        */
        Roles::CRole* nearMob = 0;
        int nearDist = -1;
        for (int i = 0; i < _client->NumMobs(); ++i)
        {
            Roles::CRole* mob;
            mob = _client->GetMob(i);
            if (_targets.size() != 0 && _targets.find(string(mob->Name())) == _targets.end())
            {
                delete mob;
                continue;
            }
            if (_mobsToIgnore.find(mob->UniqueId()) != _mobsToIgnore.end())
            {
                delete mob;
                continue;
            }
            if (mob->AnimationFlag() == 0x20)
            {
                delete mob;
                continue;
            }
            int mx, my;
            mob->GetCoords(&mx, &my);
            int dx, dy;
            dx = mx - _px;
            dy = my - _py;
            int dist = dx*dx + dy*dy;
            if (dist > 12*12)
            {
                delete mob;
                continue;
            }
            if (nearMob == 0 || dist < nearDist)
            {
                nearMob = mob;
                nearDist = dist;
            }
            else
                delete mob;
        }
        if (nearMob != 0)
        {
            _mobsToIgnore.insert(nearMob->UniqueId());
            _client->Hero()->Melee2(nearMob);
            int mx, my;
            nearMob->GetCoords(&mx, &my);
            _px = mx+1;
            _py = my+1;
            delete nearMob;
        }
        else
            _mobsToIgnore.clear();
    }
    else
        _first = true;
}

void CAttackSpammer::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg.find("#aspam") == 0)
    {
        string t;
        istringstream is(msg);
        is >> t;
        _targets.clear();
        while (is >> t)
            _targets.insert(t);
    }
}

CAttackSpammer::CAttackSpammer(CClient* client): _client(client), _lastTime(GetTickCount()), _first(true)
{
    _client->OnUpdate += new CEvent::T<CAttackSpammer>(this, &CAttackSpammer::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CAttackSpammer>(this, &CAttackSpammer::_Command);
}
