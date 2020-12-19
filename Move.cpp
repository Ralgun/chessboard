#include "Move.hpp"

bool Move::operator==(const Move& r)
{
    return (r.xStart == xStart) && (r.yStart == yStart) && (r.xEnd == xEnd) && (r.yEnd == yEnd) &&
        (r.promotion == promotion);
}