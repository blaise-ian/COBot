#ifndef COORD_H_INCLUDED
#define COORD_H_INCLUDED

namespace CO {
namespace Path {

    struct SCoord
    {
        int X, Y;

        SCoord() {}
        SCoord(const SCoord& coord): X(coord.X), Y(coord.Y) {}
        SCoord(int x, int y): X(x), Y(y) {}

        float DistSquared(const SCoord& target) const
        {
            int dx = target.X - X;
            int dy = target.Y - Y;
            return dx*dx + dy*dy;
        }

        bool operator==(const SCoord& rhs) const
        {
            return X == rhs.X && Y == rhs.Y;
        }
    };

}
}

#endif // COORD_H_INCLUDED
