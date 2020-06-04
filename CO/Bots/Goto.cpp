#include "Goto.h"

#include <string>
#include <sstream>
using namespace std;
using namespace CO::Bots;

void CGoto::_Update(void* sender, void** params)
{
    if (_running)
    {
        if (GetTickCount() - _lastTime < _delay)
            return;
        _delay = 1000;
        _lastTime = GetTickCount();
        int hx, hy;
        _client->Hero()->GetCoords(&hx, &hy);
        _pathFinder->SetSource(hx, hy);
        Path::SCoord move = _pathFinder->GetMove(12.0);
        if (move.X == -1)
            _running = false;
        else
        {
            _client->Hero()->Jump(move.X, move.Y);
            if (move == _pathFinder->Target())
                _running = false;
        }
    }
}

void CGoto::_Command(void* sender, void** params)
{
    string msg = (char*)(params[1]);
    if (msg == "#goto off")
    {
        _running = false;
        _client->Hero()->BlockTalk();
    }
    else if (msg.find("#goto") == 0)
    {
        string cmd;
        int x, y, px, py;
        istringstream is(msg);
        is >> cmd >> x >> y;
        _client->Hero()->GetCoords(&px, &py);
        _pathFinder->SetTarget(x, y);
        _pathFinder->SetSource(px, py);
        if (_pathFinder->CalculatePath() > 0.0)
            _running = true;
        _client->Hero()->BlockTalk();
    }
}

CGoto::CGoto(CClient* client): _client(client), _pathFinder(0), _running(false), _lastTime(GetTickCount()), _delay(0)
{
    _client->OnUpdate       += new CEvent::T<CGoto>(this, &CGoto::_Update);
    _client->Hero()->OnTalk += new CEvent::T<CGoto>(this, &CGoto::_Command);
    _pathFinder = new Path::CPathFinder(_client);
}

CGoto::~CGoto()
{
    delete _pathFinder;
}

void CGoto::Goto(int x, int y, int delayStart)
{
    int px, py;
    _client->Hero()->GetCoords(&px, &py);
    _pathFinder->SetTarget(x, y);
    _pathFinder->SetSource(px, py);
    if (_pathFinder->CalculatePath() > 0.0)
    {
        _delay = delayStart;
        _lastTime = GetTickCount();
        _running = true;
    }
}
