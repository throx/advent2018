#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <set>

using namespace std;

const regex INPUT_REGEX("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");

struct Point
{
    Point() : Point(0, 0) {};
    Point(int x, int y) : x(x), y(y) {};
    bool operator<(const Point& other) const { return x < other.x || (x == other.x && y < other.y); }
    int x;
    int y;
};

int main()
{
    map<Point, set<int>> claims;

    string s;
    int maxx = 0;
    int maxy = 0;
    set<int> ids;
    
    while (getline(cin, s)) {
        smatch sm;
        regex_match(s.cbegin(), s.cend(), sm, INPUT_REGEX);
        int id = stoi(sm[1]);
        int x = stoi(sm[2]);
        int y = stoi(sm[3]);
        int w = stoi(sm[4]);
        int h = stoi(sm[5]);

        for (int xx = x; xx < x + w; ++xx) {
            for (int yy = y; yy < y + h; ++yy) {
                claims[Point(xx, yy)].insert(id);
            }
        }

        maxx = max(maxx, x + w);
        maxy = max(maxy, y + h);
        ids.insert(id);
    }

    int c = count_if(claims.begin(), claims.end(), [](auto x) {return x.second.size() >= 2; });
    cout << "Double claims = " << c << endl;

    for (auto claim : claims) {
        if (claim.second.size() >= 2) {
            for (auto c : claim.second) {
                ids.erase(c);
            }
        }
    }

    if (ids.size() != 1) return 1;
    cout << "Single claim = " << *(ids.begin()) << endl;

    return 0;
}
