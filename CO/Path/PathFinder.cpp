#include "PathFinder.h"
#include <algorithm>
#include <vector>
#include <sstream>
#include <math.h>

using namespace CO::Path;
using namespace std;

static const float sqrt2 = sqrt(2);

// TODO: Test this.
float CPathFinder::_Calculate()
{
    _client->FreeGround(&(_target.X), &(_target.Y));
    if (_client->ReadGround(_source.X, _source.Y) != 0)
        return -1.0;
    if (_client->ReadGround(_target.X, _target.Y) != 0)
        return -1.0;
    float best_distance = -1.0;
    ltCoordEstDist ltCoord(this);
    /*
    _target.X >>= 1; _target.X <<= 1;
    _target.Y >>= 1; _target.Y <<= 1;
    _source.X >>= 1; _source.X <<= 1;
    _source.Y >>= 1; _source.Y <<= 1;
    */
    _visitedNodes[_target] = 0.0;
    if (_target == _source)
        return 0.0;
    vector<SCoord> nodes;
    nodes.push_back(_target);
    push_heap(nodes.begin(), nodes.end(), ltCoord);
    while (nodes.size() > 0)
    {
        pop_heap(nodes.begin(), nodes.end(), ltCoord);
        SCoord coord = nodes.back();
        nodes.pop_back();

        //ostringstream os;
        //os << "exploring node (" << coord.X << ", " << coord.Y << ")";
        //MessageBox(0, os.str().c_str(), "info", MB_OK);

        SCoord children[8];
        children[0] = SCoord(coord.X-1, coord.Y-1);
        children[1] = SCoord(coord.X, coord.Y-1);
        children[2] = SCoord(coord.X+1, coord.Y-1);
        children[3] = SCoord(coord.X-1, coord.Y);
        children[4] = SCoord(coord.X+1, coord.Y);
        children[5] = SCoord(coord.X-1, coord.Y+1);
        children[6] = SCoord(coord.X, coord.Y+1);
        children[7] = SCoord(coord.X+1, coord.Y+1);

        for (int i = 0; i < 8; ++i)
        {
            if (_client->ReadGround(children[i].X, children[i].Y) != 0)
                continue;
            float cdist = _visitedNodes[coord] + sqrt(coord.DistSquared(children[i]));
            if (best_distance > 0.0 && cdist > best_distance)
                continue;
            if (_visitedNodes.find(children[i]) == _visitedNodes.end() || cdist < _visitedNodes[children[i]])
            {
                _visitedNodes[children[i]] = cdist;
                nodes.push_back(children[i]);
                push_heap(nodes.begin(), nodes.end(), ltCoord);
            }
            if (children[i] == _source)
            {
                if (best_distance < 0.0 || cdist < best_distance)
                {
                    best_distance = cdist;
                }
                nodes.clear();
                break;
            }
        }
    }

    //MessageBox(0, "done", "info", MB_OK);

    return best_distance;
}

void CPathFinder::_GetMove(float stepSize, int x, int y, SCoord* res)
{
    SCoord coord(x,y);

    if (x == _target.X && y == _target.Y)
    {
        res->X = x;
        res->Y = y;
        return;
    }

    float dist;
    int dx, dy;
    dx = x - _source.X;
    dy = y - _source.Y;
    dist = sqrt(dx*dx + dy*dy);
    if (dist >= stepSize)
    {
        res->X = x;
        res->Y = y;
        return;
    }

    vector<SCoord> children(8);

    children[0] = SCoord(coord.X-1, coord.Y-1);
    children[1] = SCoord(coord.X, coord.Y-1);
    children[2] = SCoord(coord.X+1, coord.Y-1);
    children[3] = SCoord(coord.X-1, coord.Y);
    children[4] = SCoord(coord.X+1, coord.Y);
    children[5] = SCoord(coord.X-1, coord.Y+1);
    children[6] = SCoord(coord.X, coord.Y+1);
    children[7] = SCoord(coord.X+1, coord.Y+1);

    SCoord nextCoord;
    dist = -1.0;
    for (int i = 0; i < 8; ++i)
    {
        float cdist = -1.0;
        if (_visitedNodes.find(children[i]) != _visitedNodes.end())
            cdist = _visitedNodes[children[i]];
        if (cdist < 0.0)
            continue;
        if (dist < 0.0 || cdist < dist)
        {
            nextCoord = children[i];
            dist = cdist;
        }
    }

    if (dist < 0.0)
    {
        res->X = -1;
        res->Y = -1;
        return;
    }

    _GetMove(stepSize, nextCoord.X, nextCoord.Y, res);
}

SCoord CPathFinder::GetMove(float stepSize)
{
    SCoord res(-1,-1);
    //_GetMove(stepSize, (_source.X >> 1) << 1, (_source.Y >> 1) << 1, &res);
    _GetMove(stepSize, _source.X, _source.Y, &res);
    return res;
}
