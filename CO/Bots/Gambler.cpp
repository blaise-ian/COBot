#include "Gambler.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace CO::Bots;
using namespace std;

void CGambler::_Role(void* sender, void** params)
{
    int d1, d2, d3;
    d1 = (int)(params[0]);
    d2 = (int)(params[1]);
    d3 = (int)(params[2]);
    if (d1 + d2 + d3 > 10)
    {
        if (_history[_cell] > _history[_cell+1])
            _amount <<= 1;
        else
            _amount = 1;
        ++_cell;
    }
    else
    {
        if (_history[_cell] > _history[_cell+1])
            _amount = 1;
        else
            _amount <<= 1;
    }
    _history[_cell] += _amount;
    _cell &= 7;
    _cell <<= 1;
    _betMade = false;
    _lastRoleTime = GetTickCount();

    string fname = _baseDir + "\\dicelog.txt";
    ofstream os2(fname.c_str(), ios::app | ios::out);
    os2 << d1 << " " << d2 << " " << d3 << endl;

    fname = _baseDir + "\\diceking.txt";
    ofstream os(fname.c_str());
    for (int i = 0; i < 16; ++i)
    {
        os << ' ' << _history[i];
    }
    os << endl;
    os << " \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ /" << endl;
    os << "  \\   /   \\   /   \\   /   \\   /" << endl;
    os << "   \\ /     \\ /     \\ /     \\ /" << endl;
    os << "    V       V       V       V" << endl;
    os << "     \\      /        \\      /" << endl;
    os << "      \\    /          \\    /" << endl;
    os << "       \\  /            \\  /" << endl;
    os << "        \\/              \\/" << endl;
    os << "         \\              /" << endl;
    os << "          \\            /" << endl;
    os << "           \\          /" << endl;
    os << "            \\        /" << endl;
    os << "             \\      /" << endl;
    os << "              \\    /" << endl;
    os << "               \\  /" << endl;
    os << "                \\/" << endl;
    os.close();
}

void CGambler::_Update(void* sender, void** params)
{
    if (_running && !_betMade && (GetTickCount() - _lastRoleTime) >= 10000)
    {
        if (_history[_cell] > _history[_cell+1])
        {
            for (int i = 0; i < _amount; ++i)
            {
                char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00};
                //char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00};
                _client->SendPacket(packet, sizeof(packet));
            }
        }
        else
        {
            for (int i = 0; i < _amount; ++i)
            {
                char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00};
                //char packet[] = {0x14, 0x00, 0x59, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00};
                _client->SendPacket(packet, sizeof(packet));
            }
        }
        _betMade = true;
    }
}

void CGambler::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#gamble off")
    {
        _running = false;
    }
    else if (msg.find("#gamble") == 0)
    {
        _amount = 1;
        _lastRoleTime = GetTickCount();
        _betMade = false;
        _running = true;
    }
}

CGambler::CGambler(CClient* client, const char* dir): _client(client), _cell(0), _baseDir(dir)
{
    _client->OnDiceKingRole += new CEvent::T<CGambler>(this, &CGambler::_Role);
    _client->OnUpdate += new CEvent::T<CGambler>(this, &CGambler::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CGambler>(this, &CGambler::_Command);
    for (int i = 0; i < 16; ++i)
        _history[i] = 0;
    string fname = _baseDir + "\\diceking.txt";
    ifstream is(fname.c_str());
    if (is)
    {
        for (int i = 0; i < 16; ++i)
            is >> _history[i];
    }
}
