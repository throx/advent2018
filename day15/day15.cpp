#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <map>

#include "../shared/Point.h"

using namespace std;

// A unit on the board
struct Unit {
    Unit(Point2 loc, char type, size_t id, int atk) : loc(loc), type(type), atk(atk), hp(200), id(id) {};
    Point2 loc;
    char type;
    int atk;
    int hp;
    size_t id;
};

// The world
struct World {
    vector<string> rep;  // The map - as input
    vector<Unit> units;  // List of units on the board.
    int elf_attack;      // Elf attack value

    World(const vector<string>& rep, int ea) : elf_attack(ea), rep(rep)
    {
        // Parse the input to fill the unit list.
        for (int y = 0; y < my(); ++y) {
            for (int x = 0; x < mx(); ++x) {
                switch (at(x, y)) {
                case 'G':
                {
                    Unit u({ x, y }, at(x, y), units.size(), 3);
                    units.push_back(u);
                    break;
                }
                case 'E':
                {
                    Unit u({ x, y }, at(x, y), units.size(), elf_attack);
                    units.push_back(u);
                    break;
                }
                }
            }
        }
    }

    // Accessors
    char at(const Point2& p) const { return rep[p[1]][p[0]]; }
    char at(int x, int y) const { return rep[y][x]; }
    size_t mx() const { return rep[0].length(); }
    size_t my() const { return rep.size(); }

private:
    // Setters are private
    void set(const Point2& p, char c) { rep[p[1]][p[0]] = c; }
    void set(int x, int y, char c) { rep[y][x] = c; }

public:
    // Move a unit to the position
    void MoveUnit(size_t id, Point2 loc) {
        set(units[id].loc, '.');
        set(loc, units[id].type);
        units[id].loc = loc;
    }

    // Unit attacks someone
    void Attack(size_t target, size_t attacker) {
        units[target].hp = units[target].hp - units[attacker].atk;
        if (IsDead(target)) {
            // Killed
            set(units[target].loc, '.');
        }
    }

    // True if a unit is dead
    bool IsDead(size_t id) {
        return units[id].hp <= 0;
    }
};

// Dump the world, for debugging
void DumpWorld(const World& world) {
    for (int y = 0; y < world.my(); ++y) {
        cout << world.rep[y];

        for (auto& u : world.units) {
            if (u.loc[1] == y && u.hp > 0) {
                cout << "   " << u.type << u.loc << "=" << u.hp;
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Sorter for points
struct reading_order_p {
    bool operator()(const Point2& l, const Point2& r) const {
        return (l[1] == r[1]) ? l[0] < r[0] : l[1] < r[1];
    }
};

// Sorter for units
struct reading_order_u {
    static reading_order_p rop;
    bool operator()(const Unit& l, const Unit& r) const {
        return rop(l.loc, r.loc);
    }
};
reading_order_p reading_order_u::rop;

// Units set, sorted by reading order
typedef set<Unit, reading_order_u> Units;

// Get the units from the world into a sorted set
Units GetUnits(const World& world)
{
    Units ret;
    for (const auto& u : world.units) {
        if (u.hp > 0) {
            ret.insert(u);
        }
    }
    return ret;
}

// Distance between units
__int64 Dist(const Unit& u1, const Unit& u2) {
    return Dist(u1.loc, u2.loc);
}

// Generate all distances in the world from a point, NOT including the point.
map<Point2, int> Dijkstra(const World& world, const Unit& from)
{
    map<Point2, int> dists;
    vector<Point2> last_added;
    last_added.reserve(4);
    last_added.push_back(from.loc);
    vector<Point2> now_added;
    now_added.reserve(4);

    __int64 maxx = world.mx();
    __int64 maxy = world.my();
    int d = 0;

    // Loop, adding the "n+1" distance around each "n" distance.  Breadth-first floodfill-ish.
    bool finish = false;
    while (!finish) {
        finish = true;
        for (auto& p : last_added) {

            // For each neighbour
            p.DoSquareNeighbours([&](const Point2& newp) {

                    // If out of bounds, do nothing
                    if (newp[0] < 0 || newp[0] >= maxx || newp[1] < 0 || newp[1] >= maxy) return;

                    // If not empty, do nothing
                    if (world.at(newp) != '.') return;

                    // Find or insert the point in the distance map
                    auto ins = dists.insert(make_pair(newp, 0));

                    // If we found it, do nothing (already filled by a shorter route)
                    if (!ins.second) return;

                    // Add the distance + 1 to the map
                    ins.first->second = d + 1;

                    // Remember this is a new one
                    now_added.push_back(newp);

                    // Not finished yet!
                    finish = false;
                });
        }
        ++d;

        // Stash the newly added ones into the last added and clear out the new list
        now_added.swap(last_added);
        now_added.clear();
    }

    return dists;
}

// Trace back a point to its source, and report the first step on the way.  Quicker than revers floodfill.
Point2 FirstStep(const map<Point2, int>& dists, Point2 dest)
{
    // Cheat and sort in reading order
    vector<Point2> DIRS = {
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1}
    };

    auto it = dists.find(dest);
    if (it == dists.end()) throw "Boom";
    int d = it->second;

    // Step through finding the first item of lesser distance.  This will always work because of the way
    // we construct the distances - each one is necessarily next to at least one of the distance n-1.
    while (d > 1) {
        --d;

        Point2 newdest = dest;
        for (const auto& x : DIRS) {
            auto p = dest + x;
            it = dists.find(p);
            if (it != dists.end()) {
                if (it->second == d) {
                    newdest = p;
                    break;
                }
            }
        }
        if (newdest == dest) throw "BOOM";
        dest = newdest;
    }

    return dest;
}

// Do one turn in the world, return true if still a "live" world.
bool DoTurn(World& world)
{
    auto units = GetUnits(world);
    for (auto& unit : units) {

        // Check we are still alive!
        if (world.IsDead(unit.id)) continue;

        set<Point2, reading_order_p> moveopts;
        bool move = true;

        // Identify all targets (may have moved, so get them again)
        auto targets = GetUnits(world);
        for (auto& target : targets) {
            if (target.type == unit.type) continue;

            // Are we "in range"
            if (Dist(unit, target) == 1) {
                move = false;
            }
            else {
                // Collect moving spots
                target.loc.DoSquareNeighbours([&](const Point2& p) { moveopts.insert(p); });
            }
        }

        // Do we have to move (ie not immediate attack)
        if (move) {

            // If nowhere to move to, then must have no enemies, so we're done.  World is dead.
            if (moveopts.empty()) {
                return false;
            }

            // Plot all distances in the world
            auto dists = Dijkstra(world, unit);

            // Add all move options to the map, to ensure lookups succeed in next step.
            for (auto& p : moveopts) {
                if (dists.find(p) == dists.end()) {
                    dists[p] = INT_MAX;
                }
            }

            // Find the shortest distance to all the move options
            auto ele = min_element(moveopts.begin(), moveopts.end(), [&](const auto& a, const auto& b) { return dists[a] < dists[b]; });

            // If we can get there then step there, if not, the we don't go to the next one we just stay still
            if (dists[*ele] != INT_MAX) {

                // Get the first step
                auto step = FirstStep(dists, *ele);

                // Move the unit
                world.MoveUnit(unit.id, step);
            }
        }

        // Now we've moved, identify all attackable targets
        Units victims;
        for (auto& target : targets) {
            if (target.type == unit.type) continue;

            // Are we "in range"?
            if (Dist(world.units[unit.id], target) == 1) {
                victims.insert(target);
            }
        }

        // If there's a potential victim,
        if (!victims.empty()) {

            // Find the one with the least hp
            auto ele = min_element(victims.begin(), victims.end(), [](const auto& a, const auto& b) { return a.hp < b.hp; });

            // Attack the victim!
            world.Attack(ele->id, unit.id);
        }

    }

    // World is still alive
    return true;
}

int main()
{
    // Read the input
    vector<string> input;
    string s;
    while (getline(cin, s)) {
        input.push_back(s);
    }

    // Elves start with attack of 3.  Guess 16 increment.
    int atk = 3;
    int inc = 16;

    {
        // Create world
        World world(input, atk);

        // Play the game
        int t = 0;
        while (DoTurn(world)) ++t;

        // Compute result for part 1
        __int64 part1 = 0;
        for (auto& u : world.units) {
            if (!world.IsDead(u.id)) {
                part1 += u.hp;
            }
        }
        part1 = part1 * t;
        cout << "Part 1 = " << part1 << endl;
    }

    // Loop to find minimal version with no elves dying:
    // - (Assume atk = 3 fails, because that's what the instructions said)
    // - Increment atk by inc.
    // - If an elf dies, we're still too low, so continue loop from start
    // - If no elf dies, we might be too high:
    // -   If the increment is 1, then we know that any atk with a lower number kills an elf, so we're done.
    // -   Otherwise, put atk back to the lower number (which we know is too low), halve the increment and try again.
    while (true) {
        atk += inc;

        // Play the game
        World world(input, atk);
        int t = 0;
        while (DoTurn(world)) ++t;

        // I compute the winner, just for infos.
        char winner = 'X';
        int deadelves = 0;
        __int64 part2 = 0;
        for (auto& u : world.units) {
            if (!world.IsDead(u.id)) {
                part2 += u.hp;
                winner = u.type;
            }
            else {
                if (u.type == 'E') {
                    ++deadelves;
                }
            }
        }
        part2 = part2 * t;

        cout << "Attack " << atk << " Winner = " << string({ winner }) << " Dead Elves = " << deadelves << endl;
        if (deadelves == 0) {
            if (inc == 1) {
                cout << "Part 2 = " << part2 << endl;
                return 0;
            }

            atk -= inc;
            inc = inc >> 1;
        }
    }
}
