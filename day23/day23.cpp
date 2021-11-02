#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("pos=<([-\\d]+),([-\\d]+),([-\\d]+)>, r=(\\d+)");

struct Bot {
    int id;
    Point3 pos;
    int range;
};
typedef vector<Bot> Bots;

// Box - could probably be generalised in Point?
struct Box {
    Point3 p1, p2;

    bool Intersects(const Point3& p, int range);
    vector<Box> Split();
    bool Contains(const Point3& p);

    template<class _F>
    void DoCorners(_F fn) const
    {
        fn(Point3({ p1[0], p1[1], p1[2] }));
        fn(Point3({ p1[0], p1[1], p2[2] }));
        fn(Point3({ p1[0], p2[1], p1[2] }));
        fn(Point3({ p1[0], p2[1], p2[2] }));
        fn(Point3({ p2[0], p1[1], p1[2] }));
        fn(Point3({ p2[0], p1[1], p2[2] }));
        fn(Point3({ p2[0], p2[1], p1[2] }));
        fn(Point3({ p2[0], p2[1], p2[2] }));
    }
};

bool Box::Contains(const Point3& p)
{
    for (int dim = 0; dim < 3; ++dim) {
        if (p[dim] < min(p1[dim], p2[dim]) || p[dim] > max(p1[dim], p2[dim])) {
            return false;
        }
    }
    return true;
}

bool Box::Intersects(const Point3& p, int range)
{
    // If point inside, then gtg.
    if (Contains(p)) return true;

    // Either the "points" of the radar are inside the box, or the 
    // corners of the box are in range
    if (Contains(Point3({ p[0] + range, p[1], p[2] }))) return true;
    if (Contains(Point3({ p[0] - range, p[1], p[2] }))) return true;
    if (Contains(Point3({ p[0], p[1] + range, p[2] }))) return true;
    if (Contains(Point3({ p[0], p[1] - range, p[2] }))) return true;
    if (Contains(Point3({ p[0], p[1], p[2] + range }))) return true;
    if (Contains(Point3({ p[0], p[1], p[2] - range }))) return true;

    bool inside = false;
    DoCorners([&](auto c) { if (Dist(p, c) <= range) inside = true; });
    return inside;
}

// Split a box 8 ways.  Could do this with an algorithm, but for 3D just as easy to unroll
vector<Box> Box::Split()
{
    vector<Box> ret;
    ret.reserve(8);
    Point3 md = (p1 + p2) / 2;
    ret.push_back(Box({ Point3({p1[0], p1[1], p1[2]}), Point3({md[0], md[1], md[2]}) }));
    ret.push_back(Box({ Point3({p1[0], p1[1], md[2]}), Point3({md[0], md[1], p2[2]}) }));
    ret.push_back(Box({ Point3({p1[0], md[1], p1[2]}), Point3({md[0], p2[1], md[2]}) }));
    ret.push_back(Box({ Point3({p1[0], md[1], md[2]}), Point3({md[0], p2[1], p2[2]}) }));
    ret.push_back(Box({ Point3({md[0], p1[1], p1[2]}), Point3({p2[0], md[1], md[2]}) }));
    ret.push_back(Box({ Point3({md[0], p1[1], md[2]}), Point3({p2[0], md[1], p2[2]}) }));
    ret.push_back(Box({ Point3({md[0], md[1], p1[2]}), Point3({p2[0], p2[1], md[2]}) }));
    ret.push_back(Box({ Point3({md[0], md[1], md[2]}), Point3({p2[0], p2[1], p2[2]}) }));
    reverse(ret.begin(), ret.end());
    return ret;
}

// Scan a region for the best score
pair<size_t, Box> BestScore(const Box& b, const Bots& bots)
{
    Point3 best;
    size_t bestnum = 0;

    assert(b.p1[0] < b.p2[0]);
    assert(b.p1[1] < b.p2[1]);
    assert(b.p1[2] < b.p2[2]);

    for (__int64 x = b.p1[0]; x < b.p2[0]; ++x) {
        for (__int64 y = b.p1[1]; y < b.p2[1]; ++y) {
            for (__int64 z = b.p1[2]; z < b.p2[2]; ++z) {
                Point3 p({ x, y, z });
                size_t num = count_if(bots.begin(), bots.end(), [&](auto k) { return Dist(k.pos, p) <= k.range; });
                if ((num > bestnum) ||
                    (num == bestnum && Dist(p, Point3()) < Dist(best, Point3()))) {
                    best = p;
                    bestnum = num;
                }
            }
        }
    }
    return { bestnum, Box({best, best}) };
}

int main()
{
    Bots bots;
    int id = 0;
    __int64 min_coord = 0;
    __int64 max_coord = 0;

    while (!cin.eof()) {
        string s;
        smatch sm;

        getline(cin, s);
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        Point3 p({ stoi(sm[1]), stoi(sm[2]), stoi(sm[3]) });
        int r = stoi(sm[4]);

        min_coord = min({ min_coord, p[0], p[1], p[2] });
        max_coord = max({ max_coord, p[0], p[1], p[2] });

        bots.push_back({ id++, p, r });
    }

    // Find biggest, the find num in range
    auto biggest = max_element(bots.begin(), bots.end(), [](auto x, auto y) { return x.range < y.range; });
    size_t num = count_if(bots.begin(), bots.end(), [&](auto x) { return Dist(x.pos, biggest->pos) <= biggest->range; });

    cout << "Part 1 = " << num << endl;

    // Find max overlap
    multimap<size_t, Box> todo;

    // Start with a box as big as the world
    __int64 m = max(-min_coord, max_coord);
    Box start({ Point3({-m, -m, -m}), Point3({m, m, m}) });
    num = count_if(bots.begin(), bots.end(), [&](auto x) { return start.Intersects(x.pos, x.range); });
    assert(num == bots.size());
    todo.insert({ num, start });

    Box b;
    while (true) {

        // Pop the next largest box from the *top* of the map
        b = todo.rbegin()->second;
        num = todo.rbegin()->first;
        cout << "  Box size " << Dist(b.p1, b.p2) << " in " << num << endl;

        // Magic to erase a reverse iterator
        auto it = todo.rbegin();
        ++it;
        todo.erase(it.base());
        
        // Zero box = done
        auto box_size = Dist(b.p1, b.p2);
        if (box_size == 0) {
            break;
        }
        // Is box small enough to scan?
        if (box_size < 10) {

            // Have to insert the scan here, because it's possible
            // that no scan scores as high as the aggregate box,
            // so the best is elsewhere.
            todo.insert(BestScore(b, bots));

        }
        else {

            // Partition box
            auto more_boxes = b.Split();

            // Stash new partitions and their scores on the todo list
            for (auto& nb : more_boxes) {
                num = count_if(bots.begin(), bots.end(), [&](auto x) { return nb.Intersects(x.pos, x.range); });
                todo.insert({ num, nb });
            }
        }
    }

    // Loop pops out with 'b' as a single point that wins.
    assert(b.p1 == b.p2);
    cout << "Part 2 = " << Dist(b.p1, Point3()) << endl;
}
