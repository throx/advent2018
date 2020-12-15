#include <iostream>
#include <string>
#include <set>

using namespace std;

const __int64 ITERS = 50000000000LL;

void RunRules(string& state, __int64& offset, const set<string>& rules)
{
    string newstate;
    newstate.reserve(state.length() + 5);
    state.insert(0, "...."s);
    state.append("...."s);
    offset -= 2;

    for (int i = 0; i < state.length() - 5; ++i) {
        if (rules.find(state.substr(i, 5)) != rules.end()) {
            newstate.append(1, '#');
        }
        else {
            newstate.append(1, '.');
        }
    }

    __int64 a = newstate.find("#");
    __int64 b = newstate.find_last_of("#");

    state = newstate.substr(a, b-a+1LL);
    offset += a;
}

int main()
{
    string s;
    getline(cin, s);
    string state = s.substr(15);

    getline(cin, s);

    set<string> rules;

    while (getline(cin, s)) {
        if (s[9] == '#') {
            rules.insert(s.substr(0, 5));
        }
    }

    __int64 offset = 0;
    __int64 gen = 0;
    __int64 incr = 0;

    while(true) {
        string laststate = state;
        __int64 lastoffset = offset;
        RunRules(state, offset, rules);
        ++gen;
        cout << offset << " " << state << endl;
        incr = offset - lastoffset;
        if (state == laststate) break;
    }

    offset += (ITERS - gen) * incr;

    __int64 sum = 0;
    for (int i = 0; i < state.length(); ++i) {
        if (state[i] == '#') {
            sum += (i + offset);
        }
    }

    cout << "Sum = " << sum << endl;
}
