#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../shared/Point.h"

using namespace std;

typedef vector<string> Field;
const char TREE = '|';
const char OPEN = '.';
const char YARD = '#';

int Count(const Field& f) {
    int nt = 0;
    int ny = 0;

    for (int y = 0; y < f.size(); ++y) {
        for (int x = 0; x < f[0].length(); ++x) {

            char w = f[y][x];
            if (w == TREE) ++nt;
            if (w == YARD) ++ny;
        }
    }

    return nt * ny;
}

int main()
{
    Field f;
    map<Field, int> fm;

    while (!cin.eof()) {
        string s;
        getline(cin, s);
        f.push_back(s);
    }

    for (int turn = 0; turn < 1000000000; ++turn) {
        Field nf = f;
        for (int y = 0; y < f.size(); ++y) {
            for (int x = 0; x < f[0].length(); ++x) {

                int nt = 0;
                int ny = 0;

                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (dy == 0 && dx == 0) continue;

                        int x1 = x + dx;
                        int y1 = y + dy;
                        if (x1 < 0 || x1 >= f[0].length()) continue;
                        if (y1 < 0 || y1 >= f.size()) continue;

                        char w = f[y1][x1];
                        if (w == TREE) ++nt;
                        if (w == YARD) ++ny;
                    }
                }

                char here = f[y][x];
                switch (here) {
                case OPEN:
                    if (nt >= 3) nf[y][x] = TREE;
                    break;

                case TREE:
                    if (ny >= 3) nf[y][x] = YARD;
                    break;

                case YARD:
                    if (ny == 0 || nt == 0) nf[y][x] = OPEN;
                    break;
                }
            }
        }

        swap(f, nf);

        if (turn == 9) cout << "Part 1: " << Count(f) << endl;

        auto it = fm.find(f);
        if (it == fm.end()) {
            fm[f] = turn;
        }
        else {
            int period = turn - it->second;
            if (turn % period == 999999999 % period) {
                break;
            }
        }
    }

    cout << "Part 2: " << Count(f) << endl;
}
