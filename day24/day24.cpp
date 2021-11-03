#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <assert.h>
#include <sstream>

using namespace std;

const regex INPUT_R("(\\d+) units each with (\\d+) hit points(?:(?:.?.?weak to ([a-z, ]+).)|(?:.?.?immune to ([a-z, ]+).))* with an attack that does (\\d+) (\\w+) damage at initiative (\\d+)");
const string SIDE_S[] = { "Immune", "Infection" };

inline std::string trim_right_copy(
    const std::string& s,
    const std::string& delimiters = " \f\n\r\t\v")
{
    return s.substr(0, s.find_last_not_of(delimiters) + 1);
}

inline std::string trim_left_copy(
    const std::string& s,
    const std::string& delimiters = " \f\n\r\t\v")
{
    return s.substr(s.find_first_not_of(delimiters));
}

inline std::string trim_copy(
    const std::string& s,
    const std::string& delimiters = " \f\n\r\t\v")
{
    return trim_left_copy(trim_right_copy(s, delimiters), delimiters);
}
vector<string> split(string str, char c = ' ')
{
    vector<string> result;

    auto pos = str.begin();
    do
    {
        auto begin = pos;

        while (pos != str.end() && *pos != c)
            ++pos;

        result.push_back(trim_copy(string(begin, pos)));
        if (pos != str.end()) ++pos;
    } while (pos != str.end());

    return result;
}



struct Group
{
    int side{}; // 0 = Immune, 1 = Infection
    int grpnum{};
    int units{};
    int hp{};
    set<string> weak;
    set<string> immune;
    int attack{};
    string type;
    int init{};

    pair<int, int> Id() const { return { side, grpnum }; };
    string Name() const { stringstream ss; ss << SIDE_S[side] << " group " << grpnum; return ss.str(); }
    int effpwr() const { return units * attack; }

    Group() {}
};

typedef pair<int, int> GroupId;
typedef map<GroupId, Group> Groups;
typedef vector<GroupId> GroupList;
typedef set<GroupId> GroupSet;
typedef map<GroupId, GroupId> Fights;

string Name(const Groups& armies, const GroupId& id)
{
    return armies.at(id).Name();
}

int Damage(const Groups& armies, const GroupId& attacker_id, const GroupId& defender_id)
{
    auto& attacker = armies.at(attacker_id);
    auto& defender = armies.at(defender_id);
    if (attacker.side == defender.side) return -999;

    if (defender.immune.find(attacker.type) != defender.immune.end()) {
        return 0;
    }

    int dmg = attacker.attack * attacker.units;

    if (defender.weak.find(attacker.type) != defender.weak.end()) {
        dmg *= 2;
    }

    return dmg;
}

// first has more attack power than second?
bool MoreEffPower(const Groups& armies, const GroupId& f, const GroupId& s)
{
    auto& first = armies.at(f);
    auto& second = armies.at(s);
    if (first.effpwr() > second.effpwr()) return true;
    if (first.effpwr() < second.effpwr()) return false;
    return first.init > second.init;
}

int Battle(Groups armies, int boost)
{
    for (auto& a : armies) {
        if (a.second.side == 0) {
            a.second.attack += boost;
        }
    }

    while (true) {
        GroupList attack_order;
        for (auto& a : armies) {
            attack_order.push_back(a.first);
        }

        // Choose targets
        GroupSet defs(attack_order.begin(), attack_order.end());
        sort(attack_order.begin(), attack_order.end(), [&](auto& x, auto& y) { return MoreEffPower(armies, x, y); });
        Fights fights;
        for (auto& atk : attack_order) {

            // Group attacks one that it would deal most damage to
            int maxdmg = 0;
            GroupId def;
            for (auto& d : defs) {
                if (atk.first == d.first) continue;

                auto dmg = Damage(armies, atk, d);
                //cout << " " << Name(armies, atk) << " would do " << dmg << " to " << Name(armies, d) << endl;
                if (dmg == 0) {
                    continue;
                }
                if (dmg > maxdmg) {
                    maxdmg = dmg;
                    def = d;
                    continue;
                }
                if (dmg < maxdmg) {
                    continue;
                }

                auto& def_grp = armies.at(def);
                auto& test_grp = armies.at(d);

                if (def_grp.effpwr() < test_grp.effpwr()) {
                    def = d;
                    continue;
                }
                if (def_grp.effpwr() > test_grp.effpwr()) {
                    continue;
                }
                if (def_grp.init < test_grp.init) {
                    def = d;
                }
            }

            if (maxdmg > 0) {
                //cout << Name(armies, atk) << " chooses " << Name(armies, def) << endl;
                fights.insert({ atk, def });
                defs.erase(def);
            }
        }

        //cout << endl;

        // Attack
        int units_total = 0;
        sort(attack_order.begin(), attack_order.end(), [&](auto& x, auto& y) { return armies.at(x).init > armies.at(y).init; });
        for (auto& atk : attack_order) {
            if (fights.find(atk) != fights.end()) {
                GroupId def = fights[atk];
                int dmg = Damage(armies, atk, def);
                int units = min(dmg / armies.at(def).hp, armies.at(def).units);
                //cout << Name(armies, atk) << " attacks " << Name(armies, def) << " doing " << dmg << " points, killing " << units << " units." << endl;
                armies.at(def).units -= units;
                units_total += units;
            }
        }

        if (units_total == 0)
        {
            return 0;
        }

        // Clean up dead units
        for (auto& a : attack_order) {
            if (armies.at(a).units == 0) {
                armies.erase(a);
            }
        }

        // Have we a winner?
        int unitcount[] = { 0, 0 };
        for (auto& a : armies) {
            unitcount[a.second.side] += a.second.units;
        }
        if (unitcount[0] == 0) {
            //cout << SIDE_S[1] << " WINS with " << unitcount[1] << " units" << endl;
            return -unitcount[1];
        }
        if (unitcount[1] == 0) {
            //cout << SIDE_S[0] << " WINS with " << unitcount[0] << " units" << endl;
            return unitcount[0];
        }

        //cout << endl;
    }
}

int main()
{
    Groups armies;

    string s;
    getline(cin, s);
    assert(s == "Immune System:");
    int side = 0;
    int grpnum = 1;

    while (!cin.eof()) {
        smatch sm;

        getline(cin, s);
        if (s.empty()) {
            if (side == 1) break;

            getline(cin, s);
            assert(s == "Infection:");
            side = 1;
            grpnum = 1;
            continue;
        }

        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        Group g;
        g.grpnum = grpnum;
        g.units = stoi(sm[1]);
        g.hp = stoi(sm[2]);
        if (sm[3].matched) {
            auto ss = split(sm[3], ',');
            g.weak.insert(ss.begin(), ss.end());
        }
        if (sm[4].matched) {
            auto ss = split(sm[4], ',');
            g.immune.insert(ss.begin(), ss.end());
        }
        g.attack = stoi(sm[5]);
        g.type = sm[6];
        g.init = stoi(sm[7]);
        g.side = side;

        armies.insert({ g.Id(), g });

        ++grpnum;
    }

    int boost = 0;
    int step = 16;

    cout << "Part 1 = " << Battle(armies, 0) << endl;

    while (Battle(armies, step) < 0) {
        step *= 2;
        cout << "  Try step " << step << endl;
    }

    step /= 2;
    boost = step;
    while (true) {
        int res = Battle(armies, boost);
        cout << "  Boost " << boost << " = " << res << endl;
        step /= 2;
        if (step == 0) break;
        if (res > 0) {
            boost -= step;
        }
        else {
            boost += step;
        }
    }

    int res;
    while ((res = Battle(armies, boost)) <= 0) {
        cout << "  Boost " << boost << " = " << res << endl;
        ++boost;
    }
    cout << "  Boost " << boost << " = " << res << endl;
}
