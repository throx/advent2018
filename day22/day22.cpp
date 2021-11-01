#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;

const string CAVE = ".=|";
const string HOLDING = "0TC";
struct State {
    int x;
    int y;
    int holding;
    string how;

    bool operator==(const State& o) const { return x == o.x && y == o.y && holding == o.holding; }
    bool operator<(const State& o) const {
        if (y < o.y) return true;
        if (y > o.y) return false;
        if (x < o.x) return true;
        if (x > o.x) return false;
        return holding < o.holding;
    }
};

typedef vector<vector<int>> Cave;
typedef multimap<int, State> StateMap;

int Risk(const Cave& c, int x, int y)
{
    return c[y][x] % 3;
}

void AddStates(StateMap& todo, const Cave& c, int t_now, int x_new, int y_new, int h_now, int r_now, string how)
{
    if (x_new >= c[0].size()) throw "XBoom";
    if (y_new >= c.size()) throw "YBoom";

    int r_new = Risk(c, x_new, y_new);
    for (int h = 0; h < 3; ++h) {
        if (h != r_new && h != r_now) {
            int t_new = t_now + 1;
            string how_new = how;
            if (h != h_now) {
                t_new += 7;
                how_new = how_new + "+" + HOLDING[h];
            }
            todo.insert({ t_new, {x_new, y_new, h, how_new} });
        }
    }
}

void AddAllStates(StateMap& todo, const Cave& c, int t_now, State now)
{
    int r_now = Risk(c, now.x, now.y);
    if (r_now == now.holding) throw "HBoom";

    if (now.x > 0) AddStates(todo, c, t_now, now.x - 1, now.y, now.holding, r_now, now.how + ",W");
    AddStates(todo, c, t_now, now.x + 1, now.y, now.holding, r_now, now.how + ",E");
    if (now.y > 0) AddStates(todo, c, t_now, now.x, now.y - 1, now.holding, r_now, now.how + ",N");
    AddStates(todo, c, t_now, now.x, now.y + 1, now.holding, r_now, now.how + ",S");
}

int main()
{
    string s;
    getline(cin, s);
    const int depth = stoi(s.substr(7));
    getline(cin, s);
    const int tx = stoi(s.substr(8));
    const int ty = stoi(s.substr(s.find(',') + 1));

    const int vx = max(tx, ty) * 3;
    const int vy = max(tx, ty) * 3;

    Cave cave;
    cave.resize(vy);
    int risk = 0;

    for (int y = 0; y < vy; ++y) {
        cave[y].resize(vx);
        for (int x = 0; x < vx; ++x) {
            __int64 gi;
            if (x == 0 && y == 0) {
                gi = 0;
            }
            else if (x == tx && y == ty) {
                gi = 0;
            }
            else if (y == 0) {
                gi = x * 16807;
            }
            else if (x == 0) {
                gi = y * 48271;
            }
            else {
                gi = cave[y - 1][x] * cave[y][x - 1];
            }

            int el = (gi + depth) % 20183;
            cave[y][x] = el;

            if (x <= tx && y <= ty) {
                risk += Risk(cave, x, y);
            }

            if (x == tx && y == ty) {
                cout << 'T';
            }
            else if (x <= 50 && y <= ty) {
                cout << CAVE[el % 3];
            }
        }
        if (y <= ty) {
            cout << endl;
        }
    }

    cout << "Part 1 = " << risk << endl;

    // Find the time to target

    const State final = { tx, ty, 1 };
    StateMap todo;
    typedef map<State, int> DoneMap;
    DoneMap done;
    todo.insert({ 0, {0, 0, 1} });
    int besttime = 0;
    string besthow;

    while (!todo.empty()) {
        auto it = todo.begin();
        int time = it->first;
        State state = it->second;
        todo.erase(it);

        if (done.find(state) == done.end()) {
            if (state == final) {
                besttime = time;
                besthow = state.how;
                break;
            }
            done.insert({ state, time });

            AddAllStates(todo, cave, time, state);
        }
    }

    cout << "Part 2 = " << besttime << " by " << besthow << endl;
}
