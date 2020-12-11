#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <algorithm>
#include <list>

using namespace std;

const regex INPUT_R("Step (.) must be finished before step (.) can begin.");

int main()
{
    map<char, set<char>> gr;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        gr[sm[2].str()[0]].insert(sm[1].str()[0]);
        gr[sm[1].str()[0]];
    }

    auto base = gr;

    list<char> order;

    while (!gr.empty()) {
        list<char> l;
        for (auto& x : gr) {
            if (x.second.size() == 0) {
                l.push_back(x.first);
                break;
            }
        }
        for (auto& x : gr) {
            for (auto y : l) {
                x.second.erase(y);
            }
        }
        for (auto& y : l) {
            gr.erase(y);
            order.push_back(y);
        }
    }

    for (auto& x : order) {
        cout << x;
    }
    cout << endl;

    gr = base;
    // Init workers
    multimap<int, char> expires;
    expires.insert(make_pair(0, ' '));
    expires.insert(make_pair(0, ' '));
    if (gr.size() > 10) {
        expires.insert(make_pair(0, ' '));
        expires.insert(make_pair(0, ' '));
        expires.insert(make_pair(0, ' '));
    }

    // Run clock
    while (!gr.empty()) {
        auto pr = *expires.begin();
        if (pr.second != ' ') cout << "Finished " << pr.second << " at " << pr.first << endl;
        expires.erase(expires.begin());
        for (auto& x : gr) {
            x.second.erase(pr.second);
        }

        char next = ' ';
        for (auto& x : gr) {
            if (x.second.size() == 0) {
                next = x.first;
                break;
            }
        }

        int end;
        if (next != ' ') {
            if (base.size() > 10) {
                end = pr.first + next - 'A' + 61;
            }
            else {
                end = pr.first + next - 'A' + 1;
            }
            cout << "Starting " << next << " at " << pr.first << endl;
            gr.erase(next);
        }
        else {
            for (auto p2 : expires) {
                if (p2.second != ' ') {
                    end = p2.first;
                    break;
                }
            }
        }
        expires.insert(make_pair(end, next));
    }

    int maxt = 0;
    for (auto pr : expires) {
        if (pr.second != ' ') {
            cout << "Finished " << pr.second << " at " << pr.first << endl;
            maxt = max(maxt, pr.first);
        }
    }

    cout << "Finished at " << maxt << endl;
}
