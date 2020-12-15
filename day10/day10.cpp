#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

#include "../shared/shared.h"

using namespace std;

const regex INPUT_R("position=<\\s*([\\d-]+),\\s*([\\d-]+)> velocity=<\\s*([\\d-]+),\\s*([\\d-]+)>");

int Entropy(const vector<Point>& p) {
    Rect r;
    for (auto& d : p) {
        r.Expand(d);
    }
    return r.h + r.w;
}

void Dump(const vector<Point>& p) {
    Rect r(p.begin(), p.end());

    set<Point> pp(p.begin(), p.end());
    for (int y = r.y; y <= r.y + r.h; ++y) {
        for (int x = r.x; x <= r.x + r.w; ++x) {
            if (pp.find(Point(x, y)) != pp.end()) {
                cout << '#';
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

int main()
{
    vector<Point> dot, vel;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        dot.push_back(Point(stoi(sm[1]), stoi(sm[2])));
        vel.push_back(Point(stoi(sm[3]), stoi(sm[4])));
    }

    int t = 0;
    int e;
    int newe;
    vector<Point> last;
    do {
        last = dot;
        e = Entropy(dot);
        for (int i = 0; i < dot.size(); ++i) {
            dot[i] += vel[i];
        }
        newe = Entropy(dot);
        ++t;
    } while (newe < e);

    Dump(last);
    cout << "After " << t - 1 << " seconds" << endl;
}
