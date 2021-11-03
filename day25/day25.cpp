#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <set>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("\\s*(-?\\d+),(-?\\d+),(-?\\d+),(-?\\d+)");

typedef Point<4> Point4;
typedef set<Point4> PointSet;
typedef vector<PointSet> Sets;

int main()
{
    PointSet points;

    while (!cin.eof()) {
        string s;
        getline(cin, s);
        smatch sm;

        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        Point4 p({ stoi(sm[1]), stoi(sm[2]), stoi(sm[3]), stoi(sm[4]) });
        points.insert(p);
    }

    Sets s;
    for (auto& p : points) {
        PointSet newset;
        newset.insert(p);

        // Find if we eat any others
        Sets ns;

        for (auto& ps : s) {
            bool found = false;
            for (auto& pp : ps) {
                if (Dist(p, pp) <= 3) {
                    found = true;
                    break;
                }
            }

            if (found) {
                newset.insert(ps.begin(), ps.end());
            }
            else {
                ns.push_back(ps);
            }
        }
        ns.push_back(newset);

        swap(s, ns);
        cout << "  " << s.size() << endl;
    }

    cout << "Constellations = " << s.size() << endl;
}
