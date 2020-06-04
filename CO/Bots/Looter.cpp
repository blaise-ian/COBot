#include "Looter.h"

#include <ctype.h>
#include <math.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
using namespace CO::Bots;
using namespace std;

void CLooter::_Update(void* sender, void** params)
{
    if (_running)
    {
        if (GetTickCount() < _nextTime)
            return;
        if (_dropIdToLoot != 0)
        {
            int px, py;
            _client->Hero()->GetCoords(&px, &py);
            for (int i = 0; i < _client->NumGroundItems(); ++i)
            {
                int dropId, itemId, x, y;
                _client->GetGroundItem(i, &dropId, &itemId, &x, &y);
                if (px == x && py == y)
                {
                    _client->Hero()->Loot(dropId, x, y);
                    //_itemsToIgnore.insert(dropId);
                    break;
                }
            }
            _dropIdToLoot = 0;
        }
        IsLooting = false;
        /*
        for (int i = 0; i < _client->Hero()->NumInvItems(); ++i)
        {
            CInvItem* invItem = _client->Hero()->GetInvItem(i);
            int l = 0;
            if (invItem->Quality() != CInvItem::None)
            {
                if ((int)invItem->Quality() < (int)CInvItem::Unique)
                {
                    if (invItem->NumPlus() == 0)
                    {
                        int px, py;
                        _client->Hero()->GetCoords(&px, &py);
                        _recentlyDropped.push_back(SCoord(px, py));
                        char packet[] = {0x1C, 0x00, 0xF1, 0x03, 0xED, 0x25, 0xB8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x83, 0xC7, 0xA9, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
                        *(int*)&packet[4] = invItem->UniqueId();
                        *(int*)&packet[8] = l;
                        *(int*)&packet[16] = GetTickCount();
                        _client->SendPacket(packet, sizeof(packet));
                        //_client->Hero()->Drop(invItem->UniqueId(), px, py);
                        //break;
                        ++l;
                        if (l == 9) { l = 0; }
                    }
                }
            }
        }
        */
        char* cmd = "#drop";
        void* params[2] = {0, cmd};
        _Command(this, params);

        int px, py;
        _client->Hero()->GetCoords(&px, &py);
        int nearItemDropId = 0;
        int nearItemX = 0;
        int nearItemY = 0;
        int nearDist = -1;
        for (int i = 0; i < _client->NumGroundItems(); ++i)
        {
            int itemDropId;
            int itemId;
            int itemX;
            int itemY;
            int dist;
            _client->GetGroundItem(i, &itemDropId, &itemId, &itemX, &itemY);
            //if (find(_recentlyDropped.begin(), _recentlyDropped.end(), SCoord(itemX, itemY)) != _recentlyDropped.end())
            //    continue;
            if (itemId > 700000)
            {
                if (_itemsToLoot.size() != 0 && _itemsToLoot.find(itemId) == _itemsToLoot.end())
                    continue;
            }
            else
            {
                int q = itemId % 10;
                if (q == 9 && !_lootSuper)
                    continue;
                if (q == 8 && !_lootElite)
                    continue;
                if (q == 7 && !_lootUnique)
                    continue;
                if (q == 6 && !_lootRefined)
                    continue;
                if (q <= 5 && !_lootNormal)
                    continue;
            }
            //if (_itemsToIgnore.find(itemDropId) != _itemsToIgnore.end())
            //    continue;
            int dx = itemX - px;
            int dy = itemY - py;
            dist = dx*dx + dy*dy;
            if (itemDropId && (nearDist == -1 || dist < nearDist))
            {
                nearItemDropId = itemDropId;
                nearItemX = itemX;
                nearItemY = itemY;
                nearDist = dist;
            }
        }
        _dropIdToLoot = 0;
        if (nearDist >= 0)
        {
            IsLooting = true;
            if (_client->GotoBot()->Done())
            {
                if (nearDist == 0)
                {
                    _dropIdToLoot = nearItemDropId;
                    _xToLoot = nearItemX;
                    _yToLoot = nearItemY;
                }
                else
                {

                    _dropIdToLoot = nearItemDropId;
                    _xToLoot = nearItemX;
                    _yToLoot = nearItemY;

                    _client->GotoBot()->Goto(nearItemX, nearItemY);
                }
            }
            _nextTime = GetTickCount() + 300;
            /*
            if (nearDist == 0)//< 4*4)
            {
                //_client->Hero()->Loot(nearItemDropId, nearItemX, nearItemY);
                _dropIdToLoot = nearItemDropId;
                _xToLoot = nearItemX;
                _yToLoot = nearItemY;
                //_client->Hero()->Loot2(nearItemDropId);
                //_itemsToIgnore.insert(nearItemDropId);
                _nextTime = GetTickCount() + 300;//300 * (int)(sqrt(nearDist)) + 1000;
            }
            else if (nearDist < 10*10)
            {
                _client->Hero()->Jump(nearItemX, nearItemY);
                //_client->Hero()->Loot(nearItemDropId, nearItemX, nearItemY);
                _nextTime = GetTickCount() + 300;
            }
            else
            {
                int dx = nearItemX - px;
                int dy = nearItemY - py;
                float a = 10.0 / (float)(dx*dx + dy*dy);
                dx = (int)(dx * a);
                dy = (int)(dy * a);
                _client->Hero()->Jump(px + dx, py + dy);
                _nextTime = GetTickCount() + 300;
            }
            */
        }
        else
        {
            _itemsToIgnore.clear();
            IsLooting = false;
            _nextTime = GetTickCount() + 300;
        }
    }
}

void CLooter::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg.find("#loot off") == 0)
    {
        IsLooting = false;
        _itemsToLoot.clear();
        _itemsToIgnore.clear();
        _recentlyDropped.clear();
        _running = false;
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#loot") == 0)
    {
        IsLooting = false;
        istringstream is(msg);
        string cmd;
        is >> cmd;
        int itemId;
        bool error = false;
        _lootNormal = false;
        _lootRefined = false;
        _lootUnique = false;
        _lootElite = false;
        _lootSuper = false;
        _itemsToLoot.clear();
        while (!is.eof())
        {
            if (is >> itemId)
                _itemsToLoot.insert(itemId);
            else
            {
                is.clear();
                if (!(is >> cmd))
                    break;
                if (cmd == "normal")
                    _lootNormal = true;
                else if (cmd == "refined")
                    _lootRefined = true;
                else if (cmd == "unique")
                    _lootUnique = true;
                else if (cmd == "elite")
                    _lootElite = true;
                else if (cmd == "super")
                    _lootSuper = true;
                else
                {
                    error = true;
                    break;
                }
            }
        }
        is.clear();
        if (error)
        {
            //char path[_MAX_PATH];
            //GetCurrentDirectory(_MAX_PATH, path);
            //string a = path;
            //a += "\\" + cmd;
            //MessageBox(0, a.c_str(), "info", MB_OK);
            char path[_MAX_PATH];
            GetCurrentDirectory(_MAX_PATH, path);
            SetCurrentDirectory(_baseDir.c_str());
            ifstream is2(cmd.c_str());
            while (!is2.eof())
            {
                if (is2 >> itemId)
                {
                    _itemsToLoot.insert(itemId);
                }
                else
                {
                    if (!(is2 >> cmd))
                        break;
                    if (cmd == "normal")
                        _lootNormal = true;
                    else if (cmd == "refined")
                        _lootRefined = true;
                    else if (cmd == "unique")
                        _lootUnique = true;
                    else if (cmd == "elite")
                        _lootElite = true;
                    else if (cmd == "super")
                        _lootSuper = true;
                    else
                        break;
                }
            }
            is2.close();
            SetCurrentDirectory(path);
        }
        _dropIdToLoot = 0;
        _running = true;
        _nextTime = GetTickCount();
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#drop") == 0)
    {
        set<int> itemsToDrop;
        for (int i = 0; i < _client->Hero()->NumInvItems(); ++i)
        {
            CInvItem* invItem = _client->Hero()->GetInvItem(i);
            if (invItem->ItemId() < 700000)
            {
                if ((int)invItem->Quality() < (int)CInvItem::Unique)
                {
                    if (invItem->NumPlus() == 0)
                    {
                        itemsToDrop.insert(invItem->UniqueId());
                    }
                }
            }
        }
        //int j = 0, k = 0;
        int l = 0;
        char packet[] = {0x1C, 0x00, 0xF1, 0x03, 0xED, 0x25, 0xB8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x83, 0xC7, 0xA9, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        int px, py;
        _client->Hero()->GetCoords(&px, &py);
        _recentlyDropped.push_back(SCoord(px-1,py-1));
        _recentlyDropped.push_back(SCoord(px,  py-1));
        _recentlyDropped.push_back(SCoord(px+1,py-1));
        _recentlyDropped.push_back(SCoord(px-1,py  ));
        _recentlyDropped.push_back(SCoord(px,  py  ));
        _recentlyDropped.push_back(SCoord(px+1,py  ));
        _recentlyDropped.push_back(SCoord(px-1,py+1));
        _recentlyDropped.push_back(SCoord(px,  py+1));
        _recentlyDropped.push_back(SCoord(px+1,py+1));
        while (_recentlyDropped.size() > 100)
            _recentlyDropped.pop_front();
        for (set<int>::iterator i = itemsToDrop.begin(); i != itemsToDrop.end(); ++i)
        {
            //_client->Hero()->Drop(*i, px + j - 1, py + k - 1);
            //j = (j + 1) & 3;
            //if (!j)
            //    k = (k + 1) & 3;
            *(int*)&packet[4] = *i;
            *(int*)&packet[8] = l;
            *(int*)&packet[16] = GetTickCount();
            _client->SendPacket(packet, sizeof(packet));
            ++l;
            if (l == 9)
                break;
        }
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#pickup") == 0)
    {
        int px, py;
        _client->Hero()->GetCoords(&px, &py);
        for (int i = 0; i < _client->NumGroundItems(); ++i)
        {
            int dropId, itemId, x, y;
            _client->GetGroundItem(i, &dropId, &itemId, &x, &y);
            if (x == px && y == py)
            {
                _client->Hero()->Loot(dropId, x, y);
                break;
            }
        }
    }
}

CLooter::CLooter(CClient* client, const char* baseDir): _client(client), _running(false), _baseDir(baseDir), IsLooting(false)
{
    _client->OnUpdate += new CEvent::T<CLooter>(this, &CLooter::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CLooter>(this, &CLooter::_Command);
}
