#include <iostream>
#include <set>
#include <vector>

using namespace std;

int main()
{
    vector<int> f;
    int c = 0;

    while (!cin.eof()) {
        int x;
        cin >> x;
        c += x;
        f.push_back(x);
    }

    cout << "Soln 1 = " << c << endl;

    set<int> seen;
    c = 0;
    auto it = f.begin();
    while (seen.find(c) == seen.end()) {
        seen.insert(c);
        c += *it;
        ++it;
        if (it == f.end()) it = f.begin();
    }

    cout << "Soln 2 = " << c << endl;
}
