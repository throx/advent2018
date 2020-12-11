#pragma once
#include <algorithm>
#include <vector>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

struct Rect;

struct Point
{
    Point() : Point(0, 0) {};
    Point(int x, int y) : x(x), y(y) {};
    bool operator<(const Point& other) const { return x < other.x || (x == other.x && y < other.y); }
    std::vector<Point> Adj4() const;
    std::vector<Point> Adj4(Rect bounds) const;
    int x;
    int y;
};

inline int mdist(const Point& p1, const Point& p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

struct Rect {
    Rect() : Rect(0, 0, 0, 0) {};
    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};
    Rect(const Point& p1, const Point& p2) : x(std::min(p1.x, p2.x)), y(std::min(p1.y, p2.y)), w(abs(p1.x - p2.x)), h(abs(p1.y - p2.y)) {};
    bool operator<(const Rect& other) const
    {
        if (x < other.x) return true;
        if (x > other.x) return false;
        if (y < other.y) return true;
        if (y > other.y) return false;
        if (w < other.w) return true;
        if (w > other.w) return false;
        return h < other.h;
    }
    void Expand(const Point& p) {
        int x1 = std::min(x, p.x);
        int x2 = std::max(x + w, p.x);
        int y1 = std::min(y, p.y);
        int y2 = std::max(y + h, p.y);
        (*this) = Rect(Point(x1, y1), Point(x2, y2));
    }
    Point Bound(const Point& p) {
        int x1 = std::min(std::max(x, p.x), x + w);
        int y1 = std::min(std::max(y, p.y), y + h);
        return Point(x1, y1);
    }
    bool Contains(const Point& p) {
        return x <= p.x && (x + w) > p.x && y <= p.y && (y + h) > p.y;
    }
    void Grow(int dx, int dy) { x -= dx; y -= dy; w += dx * 2; h += dy * 2; }
    int x;
    int y;
    int w;
    int h;
};