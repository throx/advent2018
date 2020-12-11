#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <map>
#include <set>

#include "../shared/shared.h"

using namespace std;

const regex INPUT_R("(\\d+), (\\d+)");

Rect GetBounds(const map<Point, char>& space) {
    Rect bounds;
    for (auto& i : space) {
        if (bounds.x == 0 && bounds.y == 0) {
            bounds = Rect(i.first.x, i.first.y, 0, 0);
        }
        else {
            bounds.Expand(i.first);
        }
    }

    return bounds;
}

void Dump(const map<Point, char>& space) {
    Rect bounds = GetBounds(space);

    for (int y = bounds.y; y <= bounds.y + bounds.h; ++y) {
        for (int x = bounds.x; x <= bounds.x + bounds.w; ++x) {
            Point p(x, y);
            auto it = space.find(p);
            if (it == space.end()) {
                cout << ' ';
            }
            else {
                cout << it->second;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    map<Point, char> space;
    vector<Point> points;
    set<Point> newspace;

    string s;
    char c = 'A';
    Rect bounds;

    while (getline(cin, s)) {
        smatch sm;
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);

        Point p(stoi(sm[1]), stoi(sm[2]));
        space[p] = c;
        points.push_back(p);
        ++c;
        newspace.insert(p);
        bounds.Expand(p);
    }

    auto base = space;
    auto baseb = bounds;

    bounds.Grow(2, 2);

    bool stable = false;
    while (!stable) {
        stable = true;

        set<Point> created;
        for (auto& p : newspace) {
            char c = space[p];
            for (auto n : p.Adj4(bounds)) {
                if (created.find(n) != created.end()) {
                    if (space[n] != c) {
                        space[n] = '.';
                    }
                }
                else if (space.find(n) == space.end()) {
                    space[n] = c;
                    created.insert(n);
                    stable = false;
                }
            }
        }
        newspace = created;
        //Dump(space);
    }

    map<char, int> counts;
    for (auto& it : space) {
        ++counts[it.second];
    }

    bounds = GetBounds(space);
    for (int x = bounds.x; x <= bounds.x + bounds.w; ++x) {
        counts.erase(space[Point(x, bounds.y)]);
        counts.erase(space[Point(x, bounds.y + bounds.h)]);
    }
    for (int y = bounds.y; y <= bounds.y + bounds.h; ++y) {
        counts.erase(space[Point(bounds.x, y)]);
        counts.erase(space[Point(bounds.x + bounds.w, y)]);
    }

    auto maxel = max_element(counts.begin(), counts.end(), [](auto x, auto y) { return x.second < y.second; });
    cout << "Max Area = " << maxel->first << " with " << maxel->second << endl;

    // Stage 2
    space.clear();
    bounds = baseb;

    int total = 0;
    for (int y = bounds.y; y <= bounds.y + bounds.h; ++y) {
        for (int x = bounds.x; x <= bounds.x + bounds.w; ++x) {
            int d = 0;
            Point p(x, y);
            for (auto pit : points) {
                d += mdist(p, pit);
            }
            if (d < 10000) ++total;
        }
    }

    cout << "Total less = " << total << endl;

}
