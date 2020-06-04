#include "Botjail.h"

#include <string>
#include <iostream>
using namespace CO::Bots;
using namespace std;

void CBotJail::_Update(void* sender, void** params)
{
    if (_running)
    {

        int coords[][2] =
        {
            {258 , 170},
            {290 , 126},
            {336 , 136},
            {791 , 390},
            {779 , 558},
            {828 , 694},
            {731 , 720}
        };
        int x, y;
        //x = 450 + _case * 5;
        //y = 450 + _case * 5;
        x = coords[_case][0];
        y = coords[_case][1];
        _client->Hero()->Teleport(x, y);
        ++_case;
        if (_case == 7)
            _case = 0;
        /*
        // 18 00 15 27 <direction> <heroUid> 01 00 00 00 <timestamp> <mapId>
        char packet[] = {0x18, 0x00, 0x15, 0x27, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *(int*)&packet[8] = _client->Hero()->UniqueId();
        *(int*)&packet[16] = GetTickCount();
        _client->SendPacket(packet, sizeof(packet));
        */
    }
}

void CBotJail::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#botjail off")
    {
        _running = false;
    }
    else if (msg == "#botjail")
    {
        _case = 0;
        _running = true;
    }
}

CBotJail::CBotJail(CClient* client): _client(client), _running(false)
{
    _client->OnUpdate += new CEvent::T<CBotJail>(this, &CBotJail::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CBotJail>(this, &CBotJail::_Command);
}
