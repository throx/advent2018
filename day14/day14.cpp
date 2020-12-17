#include <iostream>
#include <string>

using namespace std;

const string NS = "864801"s;
const int NI = stoi(NS);

int main()
{
    string inp = "37"s;

    size_t e1 = 0;
    size_t e2 = 1;

    while(inp.length() < NI+10) {
        int r1 = inp[e1] - '0';
        int r2 = inp[e2] - '0';
        inp.append(to_string(r1 + r2));
        e1 = (e1 + 1 + r1) % inp.length();
        e2 = (e2 + 1 + r2) % inp.length();
    }

    cout << "Phase 1: " << inp.substr(NI, 10) << endl;

    inp = "37"s;

    e1 = 0;
    e2 = 1;
    int cl = NS.length();

    while (inp.length() <= cl || (inp.substr(inp.length() - cl, cl) != NS && inp.substr(inp.length() - cl - 1, cl) != NS)) {
        int r1 = inp[e1] - '0';
        int r2 = inp[e2] - '0';
        string a = to_string(r1 + r2);

        inp.append(to_string(r1 + r2));
        e1 = (e1 + 1 + r1) % inp.length();
        e2 = (e2 + 1 + r2) % inp.length();

        if (inp.length() > 1000000000) {
            size_t loc = inp.find(NS);
            if (loc == string::npos) {
                cout << "Boom" << endl;
                return 1;
            }
            else {
                cout << "Huh? " << loc << endl;
                break;
            }
        }
    }

    cout << "Phase 2: " << inp.find(NS) << endl;
}
