#include <iostream>

using namespace std;

//const int GSN = 42;
const int GSN = 3031;

int Power(int x, int y) {
    int rackid = x + 10;
    int p = rackid * y;
    p += GSN;
    p = p * rackid;
    p = p / 100;
    p = p % 10;
    p -= 5;
    return p;
}

int main()
{
    //std::cout << "Power at 3,5 = " << Power(3, 5) << endl;
    int maxpower = -100;
    int bestx, besty, bestsz;
    for (int sz = 0; sz < 300; ++sz) {
        cout << "...testing " << sz << endl;
            for (int x = 1; x < 300-sz; ++x) {
            for (int y = 1; y < 300-sz; ++y) {
                int powsum = 0;
                for (int dx = 0; dx < sz; ++dx) {
                    for (int dy = 0; dy < sz; ++dy) {
                        powsum += Power(x + dx, y + dy);
                    }
                }

                if (powsum > maxpower) {
                    bestx = x;
                    besty = y;
                    bestsz = sz;
                    maxpower = powsum;
                }
            }
        }
    }

    cout << "Max Power at " << bestx << "," << besty << "," << bestsz << " with power " << maxpower << endl;
}
