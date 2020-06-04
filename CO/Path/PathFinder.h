#ifndef PATHFINDER_H_INCLUDED
#define PATHFINDER_H_INCLUDED

#include <map>
#include "Coord.h"
#include "../Client.h"

namespace CO {
namespace Path {

    class CPathFinder
    {
    private:
        struct ltCoordForSet
        {
            bool operator()(const SCoord& a, const SCoord& b) const
            {
                return a.X < b.X || (a.X == b.X && (a.Y < b.Y));
            }
        };
        struct ltCoordEstDist
        {
            CPathFinder* _this;
            ltCoordEstDist(CPathFinder* This): _this(This) {}
            bool operator()(const SCoord& a, const SCoord& b) const
            {
                return _this->_visitedNodes[a] + a.DistSquared(_this->_source) > _this->_visitedNodes[b] + b.DistSquared(_this->_source);
            }
        };
        typedef std::map<SCoord, double, ltCoordForSet> NodeMap;
        CClient* _client;
        NodeMap _visitedNodes;
        SCoord _target;
        SCoord _source;
        float _Calculate();
        void _GetMove(float stepSize, int x, int y, SCoord* res);
    public:
        CPathFinder(CClient* client): _client(client), _target(0,0), _source(0,0) {}
        ~CPathFinder() {}
        void SetTarget(int x, int y) { _visitedNodes.clear(); _target.X = x; _target.Y = y; }
        void SetSource(int x, int y) { _source.X = x; _source.Y = y; }
        SCoord Target() const { return _target; }
        float CalculatePath() { return _Calculate(); } //_terminateSearch = false; return _MinDist(_source); }
        SCoord GetMove(float stepSize);
    };

}
}

#endif // PATHFINDER_H_INCLUDED
