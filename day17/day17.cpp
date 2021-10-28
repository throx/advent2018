#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <set>
#include <assert.h>
#include "../shared/Point.h"
#include "../shared/Console.h"

#define VIS

using namespace std;

typedef map<Point2, char> WorldMap;
struct World {
    WorldMap wm;
    __int64 minx = 500;
    __int64 maxx = 500;
    __int64 miny = 1000;
    __int64 maxy = 0;
};

const regex INPUT_R("([xy])\\D+(\\d*)\\D+(\\d*)\\D+(\\d*)");

void Dump(const World& w)
{
    cout << JHC::clr;

    for (__int64 y = 0; y <= w.maxy; ++y) {
        for (__int64 x = w.minx; x <= w.maxx; ++x) {
            auto it = w.wm.find(Point2({ x, y }));
            if (it != w.wm.end()) {
                cout << it->second;
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

void gotoxy(const World& w, const Point2& p) { JHC::gotoxy((WORD)(p[0] - w.minx), (WORD)p[1]); }
const Point2 X({ 1, 0 });
const Point2 Y({ 0, 1 });

const char WALL = '#';
const char WATER = '|';
const char POOL = '~';
bool IsWater(char c) { return c == WATER || c == POOL; }
bool CanPool(char c) { return c == WALL || c == POOL; }

void Fill(World& w, const Point2& p, char what)
{
    assert(p[0] >= w.minx && p[0] <= w.maxx);
    w.wm[p] = what;

#ifdef VIS
    gotoxy(w, p);
    cout << what;
#endif
}

// Returns false if can't flow to this spot
void Flow(World& w, const Point2& start)
{
    // Flow down until blocked
    Point2 pos = start;
    while (w.wm[pos] == 0) {

        Fill(w, pos, WATER);
        pos += Y;

        if (pos[1] > w.maxy) {
            return;
        }
    }

    // If we hit a flow, then do nothing
    if (w.wm[pos] == WATER) {
        return;
    }

    pos -= Y;

    // Flow sideways, until either blocked  or can flow down again
    bool flowing = false;

    while (!flowing && pos[1] >= start[1]) {
        assert(w.wm[pos] == WATER);

        Point2 p = pos - X;

        while (w.wm[p] != '#' && CanPool(w.wm[p + Y])) {
            Fill(w, p, WATER);
            p -= X;
        }
        if (w.wm[p] == 0 && w.wm[p + Y] == 0) {
            Flow(w, p);
        }

        p = pos + X;
        while (w.wm[p] != '#' && CanPool(w.wm[p + Y])) {
            Fill(w, p, WATER);
            p += X;
        }
        if (w.wm[p] == 0 && w.wm[p + Y] == 0) {
            Flow(w, p);
        }

        // Check if double bounded.  Complex flows require this step separate
        flowing = true;
        Point2 p1 = pos;
        while (w.wm[p1] == WATER || w.wm[p1] == POOL) {
            p1 -= X;
        }

        Point2 p2 = pos;
        while (w.wm[p2] == WATER || w.wm[p2] == POOL) {
            p2 += X;
        }

        if (w.wm[p1] == '#' && w.wm[p2] == '#') {
            p1 += X;
            while (p1 != p2) {
                Fill(w, p1, POOL);
                p1 += X;
            }
            flowing = false;
        }

        pos -= Y;
    }
}

int main()
{
    World w;

    while (!cin.eof()) {
        string s;
        smatch sm;

        getline(cin, s);
        if (s.empty()) break;

        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        string dir = sm[1];
        int level = stoi(sm[2]);
        int from = stoi(sm[3]);
        int to = stoi(sm[4]);

        for (int pos = from; pos <= to; ++pos) {
            Point2 p = (dir == "x") ? Point2({ level, pos }) : Point2({ pos, level });
            w.wm[p] = '#';
            w.minx = min(w.minx, p[0]);
            w.maxx = max(w.maxx, p[0]);
            w.miny = min(w.miny, p[1]);
            w.maxy = max(w.maxy, p[1]);
        }
    }
    w.wm[Point2{ 500, 0 }] = '+';

    // Allow for spilling
    w.minx -= 2;
    w.maxx += 2;



#ifdef VIS
    JHC::setfontsize(10, 10);
    JHC::setsize((WORD)max(100, w.maxx - w.minx + 5), (WORD)max(50, w.maxy + 5));
    Dump(w);
#endif

    Flow(w, Point2({ 500, 1 }));

    int count = 0;
    for (auto& i : w.wm) {
        if (i.first[1] >= w.miny && IsWater(i.second)) {
            ++count;
        }
    }

#ifdef VIS
    JHC::gotoxy(0, (WORD)(w.maxy + 1));
#endif
    cout << "Part 1: " << count << endl;

    count = 0;
    for (auto& i : w.wm) {
        if (i.first[1] >= w.miny && i.second == POOL) {
            ++count;
        }
    }

    cout << "Part 2: " << count << endl;
}

