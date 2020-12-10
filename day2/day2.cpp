#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main()
{
    map<int, int> seen;
    vector<string> ids;

    while (!cin.eof()) {
        string s;

        map<char, int> count;
        cin >> s;
        ids.push_back(s);

        for (auto c : s) {
            count[c]++;
        }
        set<int> counts;
        for (auto i : count) {
            counts.insert(i.second);
        }
        for (auto i : counts) {
            seen[i]++;
        }
    }

    cout << "2s x 3s = " << seen[2] * seen[3] << endl;

    for (int x = 0; x < ids.size() - 1; ++x)
    {
        string s1 = ids[x];
        for (int y = x + 1; y < ids.size(); ++y) {
            string s2 = ids[y];

            string s3;

            for (int z = 0; z < s1.length(); ++z) {
                if (s1[z] == s2[z]) s3.push_back(s1[z]);
            }

            if (s3.length() + 1 == s1.length()) {
                cout << "Ids = " << s1 << " " << s2 << " = " << s3 << endl;
                return 1;
            }
        }
    }

    return 0;
}
