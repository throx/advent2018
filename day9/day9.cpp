#include <iostream>
#include <deque>
#include <list>
#include <algorithm>

const int players = 435;
const int marbles = 7118400;

using namespace std;

typedef list<int> circle_t;

void clock(circle_t& circle, circle_t::iterator& p)
{
    ++p;
    if (p == circle.end()) {
        p = circle.begin();
    }
}

void anticlock(circle_t& circle, circle_t::iterator& p)
{
    if (p == circle.begin()) {
        p = circle.end();
    }
    --p;
}

int main()
{
    circle_t circle;
    circle.push_back(0);
    auto curr = circle.begin();

    __int64 scores[players];
    memset(scores, 0, sizeof(scores));
    int player = 0;

    for (int m = 1; m <= marbles; ++m) {
        if ((m % 23) != 0) {
            clock(circle, curr);
            clock(circle, curr);
            curr = circle.insert(curr, m);
        }
        else {
            scores[player] += (__int64)m;
            anticlock(circle, curr);
            anticlock(circle, curr);
            anticlock(circle, curr);
            anticlock(circle, curr);
            anticlock(circle, curr);
            anticlock(circle, curr);
            anticlock(circle, curr);
            scores[player] += *curr;
            curr = circle.erase(curr);
        }
        ++player;
        if (player == players) player = 0;
        if (m % 10000 == 0) cout << m << endl;
    }

    auto maxscore = *max_element(scores, scores + players);
    cout << "Max score " << maxscore << endl;
}
