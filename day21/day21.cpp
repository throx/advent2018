#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <set>

using namespace std;

const regex INPUT_R("(\\w+) (\\d+) (\\d+) (\\d+)");
enum OPCODES { ADDR = 0, ADDI, MULR, MULI, BANR, BANI, BORR, BORI, SETR, SETI, GTIR, GTRI, GTRR, EQIR, EQRI, EQRR };
vector<string> OPCODES_S = { "ADDR", "ADDI", "MULR", "MULI", "BANR", "BANI", "BORR", "BORI", "SETR", "SETI", "GTIR", "GTRI", "GTRR", "EQIR", "EQRI", "EQRR" };
typedef map<string, OPCODES> OpcodeMap;

struct Op {
    int opcode;
    int src1;
    int src2;
    int dest;
};

typedef vector<Op> Program;

void Run(const Op& op, vector<int>& regs)
{
    switch (op.opcode)
    {
        case ADDR:
            regs[op.dest] = regs[op.src1] + regs[op.src2];
            break;

        case ADDI:
            regs[op.dest] = regs[op.src1] + op.src2;
            break;

        case MULR:
            regs[op.dest] = regs[op.src1] * regs[op.src2];
            break;

        case MULI:
            regs[op.dest] = regs[op.src1] * op.src2;
            break;

        case BANR:
            regs[op.dest] = regs[op.src1] & regs[op.src2];
            break;

        case BANI:
            regs[op.dest] = regs[op.src1] & op.src2;
            break;

        case BORR:
            regs[op.dest] = regs[op.src1] | regs[op.src2];
            break;

        case BORI:
            regs[op.dest] = regs[op.src1] | op.src2;
            break;

        case SETR:
            regs[op.dest] = regs[op.src1];
            break;

        case SETI:
            regs[op.dest] = op.src1;
            break;

        case GTIR:
            regs[op.dest] = op.src1 > regs[op.src2] ? 1 : 0;
            break;

        case GTRI:
            regs[op.dest] = regs[op.src1] > op.src2 ? 1 : 0;
            break;

        case GTRR:
            regs[op.dest] = regs[op.src1] > regs[op.src2] ? 1 : 0;
            break;

        case EQIR:
            regs[op.dest] = op.src1 == regs[op.src2] ? 1 : 0;
            break;

        case EQRI:
            regs[op.dest] = regs[op.src1] == op.src2 ? 1 : 0;
            break;

        case EQRR:
            regs[op.dest] = regs[op.src1] == regs[op.src2] ? 1 : 0;
            break;

        default:
            throw "Boom";
            break;
    }
}

void PrintRegs(const vector<int>& regs)
{
    cout << '[';
    for (int i = 0; i < regs.size(); ++i) {
        if (i != 0) cout << ", ";
        cout << regs[i];
    }
    cout << "] ";
}

void PrintOp(const Op& op)
{
    cout << OPCODES_S[(int)op.opcode];
    cout << ' ' << op.src1 << ' ' << op.src2 << ' ' << op.dest << ' ';
}

int main()
{
    OpcodeMap om;
    for (int i = 0; i < OPCODES_S.size(); ++i) {
        om[OPCODES_S[i]] = (OPCODES)i;
    }

    Program prog;
    int bound_ip = 0;

    string s;
    while (!cin.eof()) {

        smatch sm;

        getline(cin, s);
        if (s.empty()) break;
        if (s[0] == '#') {
            bound_ip = stoi(s.substr(4));
        }
        else {
            regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
            string opcode = sm[1];
            for (auto& c : opcode) c = toupper(c);
            Op op{ om[opcode], stoi(sm[2]), stoi(sm[3]), stoi(sm[4]) };
            prog.push_back(op);
        }
    }

    int maxt = 0;
    int maxn = 0;
    int mint = 0xffffff;
    int minn = 0;

    for (int i = 0; i < 16777216; ++i) {
        
        /*
        {
            int ip = 0;
            map<int, bool> brk;
            brk[6] = true;
            brk[7] = true;
            brk[8] = true;
            brk[9] = true;
            brk[10] = true;
            brk[11] = true;
            brk[12] = true;
            brk[13] = true;
            vector<int> regs;
            regs.resize(6);
            int time = 0;

            regs[0] = i;
            while (ip >= 0 && ip < prog.size()) {
                regs[bound_ip] = ip;
                if (brk[ip]) PrintRegs(regs);
                if (brk[ip]) PrintOp(prog[ip]);
                Run(prog[ip], regs);
                if (brk[ip]) PrintRegs(regs);
                if (brk[ip]) cout << endl;
                ip = regs[bound_ip];
                ++ip;
                ++time;
                if (time > 500000) break;
            }
            cout << i << ": " << time;
        }
        */

        int r0 = i;
        int r1 = 0;
        int r2 = 0;
        int r3 = 0;
        int r5 = 0;
        int time = 0;
        const int CUTOFF = 65899;

    L6:
        r2 = r3 | 0x10000;
        r3 = 0x748b9a;
    L8:
        ++time;
        if (time > CUTOFF) goto EXIT;
        r1 = r2 & 0xff;
        r3 = r3 + r1;
        r3 = r3 & 0xffffff;
        r3 = r3 * 0x1016b;
        r3 = r3 & 0xffffff;
        if (256 > r2) goto L28;
        r2 = r2 / 0x100;
        goto L8;
    L28:
        if (r3 != r0) goto L6;

    EXIT:
        if (time <= CUTOFF) {
            if (time < mint) {
                mint = time;
                minn = i;
            }
            if (time > maxt) {
                maxt = time;
                maxn = i;
            }
            cout << i << ": " << time << " min = " << mint << "@" << minn << " max = " << maxt << "@" << maxn << endl;
        }
    }

    /*
    * Oh, FFS.  I'm an idiot.  The thing starts at zero and then
    * plays around a ring of numbers in 24 bit space.  Minimal count
    * must necessarily be the "next" number from zero and maximal will be
    * whatever number precedes that when you walk around the ring, which
    * might be zero again (if 0 is in the ring), but probably won't be.
    */
}
