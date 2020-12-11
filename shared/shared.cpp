// shared.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "shared.h"

std::vector<Point> Point::Adj4() const
{
    std::vector<Point> adj;
    adj.push_back(Point(x - 1, y));
    adj.push_back(Point(x + 1, y));
    adj.push_back(Point(x, y - 1));
    adj.push_back(Point(x, y + 1));
    return adj;
}

std::vector<Point> Point::Adj4(Rect bounds) const
{
    std::vector<Point> adj;
    for (auto& p : Adj4())
    {
        if (bounds.Contains(p)) adj.push_back(p);
    }
    return adj;
}
