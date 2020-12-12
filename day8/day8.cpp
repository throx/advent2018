#include <iostream>
#include <vector>

using namespace std;

int metasum(vector<int>::iterator& pos) {
    int subcards = *pos;
    ++pos;
    int metacount = *pos;
    ++pos;

    int s = 0;
    for (int card = 0; card < subcards; ++card) {
        s += metasum(pos);
    }
    for (int m = 0; m < metacount; ++m) {
        s += *pos;
        ++pos;
    }

    return s;
}

int value(vector<int>::iterator& pos) {
    int subcards = *pos;
    ++pos;
    int metacount = *pos;
    ++pos;

    int v = 0;
    if (subcards == 0) {
        for (int m = 0; m < metacount; ++m) {
            v += *pos;
            ++pos;
        }

        return v;
    }

    vector<int> subvals;
    for (int card = 0; card < subcards; ++card) {
        subvals.push_back(value(pos));
    }
    for (int m = 0; m < metacount; ++m) {
        if (*pos > 0 && *pos <= subvals.size()) {
            v += subvals[*pos - 1];
        }
        ++pos;
    }
    return v;
}

int main()
{
    vector<int> nums;
    while (!cin.eof()) {
        int x;
        cin >> x;
        nums.push_back(x);
    }

    auto pos = nums.begin();
    cout << "Meta sum = " << metasum(pos) << endl;

    pos = nums.begin();
    cout << "Value = " << value(pos) << endl;
}
