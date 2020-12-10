#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

const regex TIME_REGEX("\\[(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\]");
const regex GUARD_REGEX("Guard #(\\d+)");
const regex SLEEP_REGEX(":(\\d\\d)\\] falls asleep");
const regex WAKE_REGEX(":(\\d\\d)\\] wakes up");

int main()
{
    string s;
    map<__int64, string> data;

    // Sort the input
    while (getline(cin, s)) {
        smatch sm;
        regex_search(s.cbegin(), s.cend(), sm, TIME_REGEX);
        __int64 t = stoll(sm[1].str() + sm[2].str() + sm[3].str() + sm[4].str() + sm[5].str());
        data[t] = s;
    }

    int guard = 0;
    int start = 0;

    map<int, int[60]> schedule;

    for (auto it : data) {
        s = it.second;
        smatch sm;
        if (regex_search(s.cbegin(), s.cend(), sm, GUARD_REGEX)) {
            guard = stoi(sm[1]);
        }
        else if (regex_search(s.cbegin(), s.cend(), sm, SLEEP_REGEX)) {
            start = stoi(sm[1]);
        }
        else if (regex_search(s.cbegin(), s.cend(), sm, WAKE_REGEX)) {
            int end = stoi(sm[1]);
            for (int m = start; m != end; ++m) {
                schedule[guard][m]++;
            }
        }
    }

    map<int, int> sleepmins;
    for (auto it : schedule) {
        sleepmins[it.first] = accumulate(it.second, it.second + 60, 0);
    }

    auto m = max_element(sleepmins.begin(), sleepmins.end(), [](auto x, auto y) { return x.second < y.second; });
    cout << "Guard " << m->first << " sleeps " << m->second << " minutes." << endl;

    auto minute = max_element(schedule[m->first], schedule[m->first] + 60);
    int sleepiest = (minute - schedule[m->first]);
    cout << "  Sleepiest minute = " << sleepiest << endl;

    cout << "  Product = " << m->first * sleepiest << endl;

    map<int, int> sleepymin;
    for (auto it : schedule) {
        sleepymin[it.first] = max_element(it.second, it.second + 60) - it.second;
    }

    auto soln2 = max_element(sleepymin.begin(), sleepymin.end(), [&](auto x, auto y) { return schedule[x.first][x.second] < schedule[y.first][y.second]; });

    cout << endl;
    cout << "Guard " << soln2->first << " sleeps " << schedule[soln2->first][soln2->second] << " minutes on minute " << soln2->second << endl;
    cout << "  Product = " << soln2->first * soln2->second << endl;

    return 0;
}
