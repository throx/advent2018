#include <iostream>
#include <string>
#include <assert.h>
#include <regex>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include "../shared/Point.h"

using namespace std;

typedef set<Point2> PSet;
typedef map<Point2, PSet> PGraph;
typedef map<Point2, int> DistMap;

//const regex PARENS_R("^(.*?)\\((.*)\\)(.*?)$");
void traverse(PGraph& pg, Point2 loc, string rx)
{
    int pos = 0;
    Point2 start = loc;

    // Lets start recursively parsing
    while (pos < rx.length()) {
        switch (rx[pos]) {
            case '(':
                {
                    string sub;
                    int nest = 1;
                    ++pos;
                    while (rx[pos] != ')' || nest > 1) {
                        if (rx[pos] == '(') ++nest;
                        if (rx[pos] == ')') --nest;
                        sub += rx[pos];
                        ++pos;
                    }

                    traverse(pg, loc, sub);
                }
                break;

            case '|':
                loc = start;
                break;

            default:
                {
                    Point2 last = loc;
                    loc += P2::DIRS[rx[pos]];
                    pg[last].insert(loc);
                    pg[loc].insert(last);
                }
                break;
        }

        ++pos;
    }
}

DistMap maxdist(const PGraph& pg, Point2 loc)
{
    DistMap dists;
    PSet curr({ loc });
    int dist = 0;

    while (!curr.empty()) {
        PSet next;
        for (Point2 p : curr) {
            if (dists.find(p) == dists.end()) {
                dists[p] = dist;
                auto& n = pg.find(p)->second;
                next.insert(n.begin(), n.end());
            }
        }

        swap(curr, next);
        ++dist;
    }

    return dists;
}

void Dump(const PGraph& pg)
{
    for (auto it : pg) {
        cout << it.first << " -> ";
        for (auto p : it.second) {
            cout << p << " ";
        }
        cout << endl;
    }
}

int main()
{
    while (!cin.eof()) {
        string rx;
        getline(cin, rx);

        assert(rx[0] == '^');
        assert(rx[rx.length() - 1] == '$');

        PGraph pg;

        traverse(pg, Point2(), rx.substr(1, rx.length() - 2));
        //Dump(pg);

        auto dm = maxdist(pg, Point2());
        int maxdist = max_element(dm.begin(), dm.end(), [](auto x, auto y) {return x.second < y.second; })->second;
        cout << "Part 1: " << maxdist << endl;

        int part2 = count_if(dm.begin(), dm.end(), [](auto x) {return x.second >= 1000; });
        cout << "Part 2: " << part2 << endl;
    }
}
