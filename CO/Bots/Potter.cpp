#include "Potter.h"

#include <string>
#include <sstream>
using namespace CO::Bots;
using namespace std;

void CPotter::_Update(void* sender, void** params)
{
    if (_pottingHp && _client->Hero()->CurrentHp() < _hpLevel)
    {
        _client->FKey(_hpFKey);
    }
    /*
    if (_pottingMp && _client->Hero()->CurrentMp() < _mpLevel)
    {
        _client->FKey(_mpFKey);
    }
    */
}

void CPotter::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#autohp off")
    {
        _pottingHp = false;
    }
    else if (msg.find("#autohp") == 0)
    {
        string cmd;
        istringstream is(msg);
        is >> cmd >> _hpLevel >> _hpFKey;
        _pottingHp = true;
    }
    /*
    else if (msg == "#automp off")
    {
        _pottingMp = false;
    }
    else if (msg.find("#automp") == 0)
    {
        string cmd;
        istringstream is(msg);
        is >> cmd >> _mpLevel >> _mpFKey;
        _pottingMp = true;
    }
    */
}

CPotter::CPotter(CClient* client): _client(client), _pottingHp(false), _pottingMp(false)
{
    _client->OnUpdate += new CEvent::T<CPotter>(this, &CPotter::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CPotter>(this, &CPotter::_Command);
}
