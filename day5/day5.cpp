// day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <algorithm>

using namespace std;

int react(list<char> s)
{
    auto i = s.begin();
    auto j = i;

    while (j != s.end()) {
        ++j;
        if (j == s.end()) break;

        if (islower(*i) && isupper(*j) && toupper(*i) == *j) {
            s.erase(i);
            s.erase(j);
            i = s.begin();
            j = i;
        }
        else if (isupper(*i) && islower(*j) && tolower(*i) == *j) {
            s.erase(i);
            s.erase(j);
            i = s.begin();
            j = i;
        }
        else {
            ++i;
        }
    }

    return s.size();
}

int main()
{
    string polymer;
    cin >> polymer;

    list<char> base(polymer.begin(), polymer.end());

    cout << "Final Polymer Len = " << react(base) << endl;

    int bestlen = polymer.length();
    char bestchar;
    for (char x = 'a'; x <= 'z'; ++x) {
        list<char> test = base;
        test.remove_if([&](auto i) {return x == tolower(i); });
        int l = react(test);
        if (l < bestlen) {
            bestlen = l;
            bestchar = x;
        }
    }

    cout << "Best improved Polymer removes " << bestchar << " for len " << bestlen << endl;
}

