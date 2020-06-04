#include "Melee.h"

#include <math.h>
#include <string>
#include <sstream>
using namespace CO::Bots;
using namespace CO::Roles;
using namespace std;

void CMelee::_Update(void* sender, void** params)
{
    if (_running && !_client->LooterBot()->IsLooting && _client->GotoBot()->Done())
    {
        if (GetTickCount() < _nextTime)
            return;
        CHero* hero = _client->Hero();
        int px, py;
        if (_client->XpSkillBot()->IsFatalStrikeOn)
        {
            if (_first)
            {
                hero->GetCoords(&_px, &_py);
                _first = false;
            }
            px = _px; py = _py;
        }
        else
        {
            hero->GetCoords(&px, &py);
            _first = true;
        }
        int    nearDist = 30*30;
        CRole* nearRole = 0;
        for (int i = 0; i < _client->NumMobs(); ++i)
        {
            int mx, my, dx, dy, dist;
            CRole* mob = _client->GetMob(i);
            if (_mobsToIgnore.find(mob->UniqueId()) != _mobsToIgnore.end())
            {
                delete mob;
                continue;
            }
            mob->GetCoords(&mx, &my);
            if (_client->ReadGround(mx, my) != 0)
            {
                delete mob;
                continue;
            }
            if (mob->AnimationFlag() == 0x20)
            {
                delete mob;
                continue;
            }
            if (_targets.size() != 0 && _targets.find(string(mob->Name())) == _targets.end())
            {
                delete mob;
                continue;
            }
            dx = mx - px;
            dy = my - py;
            dist = dx*dx + dy*dy;
            if (nearRole==0 || dist < nearDist)
            {
                if (nearRole != 0)
                    delete nearRole;
                nearRole = mob;
                nearDist = dist;
            }
            else
                delete mob;
        }
        if (nearRole)
        {
            if (_client->XpSkillBot()->IsFatalStrikeOn)
            {
                if (nearDist < 12*12)
                {
                    hero->Melee2(nearRole);
                    _mobsToIgnore.insert(nearRole->UniqueId());
                    if (_client->IsUnlocked())
                        _nextTime = GetTickCount() + 20;
                    else
                        _nextTime = GetTickCount() + 200;
                    _firstJump = true;
                    nearRole->GetCoords(&_px, &_py);
                    ++_px;
                    ++_py;
                }
                else
                {
                    //_client->Hero()->Jump(_px+1, _py+1);
                    int mx, my;
                    nearRole->GetCoords(&mx, &my);
                    _client->GotoBot()->Goto(mx, my, 2000);
                    _nextTime = GetTickCount() + 1000;
                    _first = true;
                    //_client->Hero()->Loot(0, mx, my);
                }
                /*
                else if (!_firstJump)
                {
                    int mx, my;
                    nearRole->GetCoords(&mx, &my);
                    int dx, dy;
                    dx = mx - px;
                    dy = my - py;
                    float a = 12.0 / sqrt(nearDist);
                    dx = (int)(dx * a);
                    dy = (int)(dy * a);
                    hero->Jump(px + dx, py + dy);
                    _nextTime = GetTickCount() + 1000;
                }
                else
                {
                    _firstJump = false;
                    _nextTime = GetTickCount() + 1000;
                }
                */
            }
            else if (nearDist > 4*4)
            {
            //else if (nearDist > 12*12)
            //{
            /*
                int mx, my;
                nearRole->GetCoords(&mx, &my);
                int dx, dy;
                dx = mx - px;
                dy = my - py;
                float a = 12.0 / sqrt(nearDist);
                dx = (int)(dx * a);
                dy = (int)(dy * a);
                hero->Jump(px + dx, py + dy);
                _nextTime = GetTickCount() + 500;
            }
            else if (nearDist > 4*4)
            {
                int mx, my;
                nearRole->GetCoords(&mx, &my);
                hero->Jump(mx+1, my+1);
                _nextTime = GetTickCount() + 500;
            */
                int mx, my;
                nearRole->GetCoords(&mx, &my);
                _client->GotoBot()->Goto(mx+1, my+1, 2000);
            }
            else
                hero->Melee(nearRole);
            delete nearRole;
        }
        else
        {
            if (_client->XpSkillBot()->IsFatalStrikeOn)
                _mobsToIgnore.clear();
            if (_wayPoints.size() == 0)
            {
                int px, py;
                int minDist=-1;
                int gotoX=0, gotoY=0;
                _client->Hero()->GetCoords(&px, &py);
                for (set<SCoord>::iterator i = _spawnSpots.begin(); i != _spawnSpots.end(); ++i)
                {
                    int dx = i->X - px;
                    int dy = i->Y - py;
                    int dist = dx*dx + dy*dy;
                    if (minDist == -1 || dist < minDist)
                    {
                        gotoX = i->X;
                        gotoY = i->Y;
                        minDist = dist;
                    }
                }
                if (minDist != -1)
                {
                    if (_client->XpSkillBot()->IsFatalStrikeOn)
                        _client->GotoBot()->Goto(gotoX, gotoY, 2000);
                    else
                        _client->GotoBot()->Goto(gotoX, gotoY);
                }
                _nextTime = GetTickCount() + 300;
            }
            else
            {
                if (_atWayPoint < (int)_wayPoints.size())
                {
                    _client->GotoBot()->Goto(_wayPoints[_atWayPoint].X, _wayPoints[_atWayPoint].Y);
                    ++_atWayPoint;
                    if (_atWayPoint >= (int)_wayPoints.size())
                        _atWayPoint = 0;
                }
                else
                    _atWayPoint = 0;
                _nextTime = GetTickCount() + 300;
            }
        }
    }
}

void CMelee::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#melee off")
    {
        _running = false;
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#melee") == 0)
    {
        string target;
        istringstream is(msg);
        is >> target;
        _targets.clear();
        while (is >> target)
            _targets.insert(target);
        _nextTime = GetTickCount();
        _firstJump = false;
        _atWayPoint = 0;
        _running = true;
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#addspawn") == 0)
    {
        string cmd;
        int x, y;
        istringstream is(msg);
        is >> cmd;
        if (is >> x >> y)
            _spawnSpots.insert(SCoord(x, y));
        else
        {
            _client->Hero()->GetCoords(&x, &y);
            _spawnSpots.insert(SCoord(x, y));
        }
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#clearspawns") == 0)
    {
        _spawnSpots.clear();
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#addwp") == 0)
    {
        string cmd;
        int x, y;
        _client->Hero()->GetCoords(&x, &y);
        istringstream is(msg);
        is >> cmd >> x >> y;
        _wayPoints.push_back(SCoord(x, y));
    }
    else if (msg.find("#clearwps") == 0)
    {
        _wayPoints.clear();
    }
}

CMelee::CMelee(CClient* client): _client(client), _running(false)
{
    _client->OnUpdate       += new CEvent::T<CMelee>(this, &CMelee::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CMelee>(this, &CMelee::_Command);
}
