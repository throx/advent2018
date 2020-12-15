#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../shared/shared.h"
#include "../shared/Console.h"

using namespace std;

struct CartInfo {
    Point dir;
    int select;

    CartInfo(const Point& p) : dir(p), select(0) {}
    CartInfo() : select(0) {}
};

vector<string> track;
map<Point, CartInfo> carts;

void DrawTrack()
{
    JHC::gotoxy(0, 0);
    cout << JHC::fg_gray;
    for (auto& s : track) {
        cout << s << endl;
    }
}

void DrawCarts()
{
    cout << JHC::fg_white;
    for (auto& cart : carts) {
        JHC::gotoxy(cart.first.x, cart.first.y);
        if (cart.second.dir == Point(0, 1)) {
            cout << 'v';
        }
        else if (cart.second.dir == Point(0, -1)) {
            cout << '^';
        }
        else if (cart.second.dir == Point(-1, 0)) {
            cout << '<';
        }
        else if (cart.second.dir == Point(1, 0)) {
            cout << '>';
        }
    }
}

void EraseCarts()
{
    cout << JHC::fg_gray;
    for (auto& cart : carts) {
        JHC::gotoxy(cart.first.x, cart.first.y);
        cout << track[cart.first.y][cart.first.x];
    }
}

int main()
{
    string s;
    while (getline(cin, s)) {
        track.push_back(s);
    }

    if (track.size() > 40) {
        JHC::setfontsize(4, 8);
    }
    JHC::setsize(max(track[0].length()+5, 80), max(track.size()+1, 25));

    for (int y = 0; y < track.size(); ++y) {
        for (int x = 0; x < track[y].length(); ++x) {
            switch (track[y][x]) {
            case 'v':
                track[y][x] = '|';
                carts[Point(x, y)] = Point(0, 1);
                break;
            case '^':
                track[y][x] = '|';
                carts[Point(x, y)] = Point(0, -1);
                break;
            case '<':
                track[y][x] = '-';
                carts[Point(x, y)] = Point(-1, 0);
                break;
            case '>':
                track[y][x] = '-';
                carts[Point(x, y)] = Point(1, 0);
                break;
            }
        }
    }

    DrawTrack();
    Point first_crash(-1, -1);

    while (carts.size() > 1) {
        DrawCarts();
        //Sleep(50);
        EraseCarts();

        auto newcarts = carts;

        for (auto& cart : carts) {
            Point newpos = cart.first;
            CartInfo newinfo = cart.second;
            newcarts.erase(newpos);
            newpos += newinfo.dir;

            if (newcarts.find(newpos) != newcarts.end()) {
                JHC::gotoxy(newpos.x, newpos.y);
                cout << JHC::fg_red << '#';
                if (first_crash == Point(-1, -1)) first_crash = newpos;
                newcarts.erase(newpos);
                carts.erase(newpos);
            }
            else {
                switch (track[newpos.y][newpos.x]) {
                case '\\':
                    swap(newinfo.dir.x, newinfo.dir.y);
                    break;
                case '/':
                    swap(newinfo.dir.x, newinfo.dir.y);
                    newinfo.dir.x = -newinfo.dir.x;
                    newinfo.dir.y = -newinfo.dir.y;
                    break;
                case '+':
                    switch (newinfo.select) {
                    case 0:
                        swap(newinfo.dir.x, newinfo.dir.y);
                        newinfo.dir.y = -newinfo.dir.y;
                        break;
                    case 2:
                        swap(newinfo.dir.x, newinfo.dir.y);
                        newinfo.dir.x = -newinfo.dir.x;
                        break;
                    }
                    newinfo.select = (newinfo.select + 1) % 3;
                    break;
                }

                newcarts[newpos] = newinfo;
            }
        }
        
        carts = newcarts;
    }

    JHC::gotoxy(0, track.size());
    cout << JHC::fg_gray << "First Crash = " << first_crash.x << ',' << first_crash.y << " Last Car = " << carts.begin()->first.x << "," << carts.begin()->first.y << "  ";
}
