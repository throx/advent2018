#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <set>

using namespace std;

const regex INPUT_R("\\D*(\\d+)\\D+(\\d+)\\D+(\\d+)\\D+(\\d+)\\D*");
enum OPCODES {ADDR = 0, ADDI, MULR, MULI, BANR, BANI, BORR, BORI, SETR, SETI, GTIR, GTRI, GTRR, EQIR, EQRI, EQRR};
vector<string> OPCODES_S = { "ADDR", "ADDI", "MULR", "MULI", "BANR", "BANI", "BORR", "BORI", "SETR", "SETI", "GTIR", "GTRI", "GTRR", "EQIR", "EQRI", "EQRR" };

struct Op {
    int opcode;
    int src1;
    int src2;
    int dest;
};

struct SampleOp {
    SampleOp() { before.resize(4); after.resize(4); }
    vector<int> before;
    vector<int> after;
    Op op;
};

typedef map<int, int> OpMap;

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

void Run(const OpMap& opmap, const Op& op, vector<int>& regs)
{
    Op realop = op;
    realop.opcode = opmap.find(realop.opcode)->second;
    //cout << OPCODES_S[realop.opcode] << " " << realop.src1 << ", " << realop.src2 << ", " << realop.dest << endl;
    Run(realop, regs);
}

int main()
{
    vector<SampleOp> samples;

    string s;
    while (!cin.eof()) {

        SampleOp sample;
        smatch sm;

        getline(cin, s);
        if (s.empty()) break;

        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        for (int i = 0; i < 4; ++i) sample.before[i] = stoi(sm[i + 1]);

        getline(cin, s);
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        sample.op.opcode = stoi(sm[1]);
        sample.op.src1 = stoi(sm[2]);
        sample.op.src2 = stoi(sm[3]);
        sample.op.dest = stoi(sm[4]);

        getline(cin, s);
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);
        for (int i = 0; i < 4; ++i) sample.after[i] = stoi(sm[i + 1]);

        samples.push_back(sample);

        getline(cin, s);
    }

    map<int, set<int>> possibles;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            possibles[i].insert(j);
        }
    }

    int part1 = 0;

    for (auto& sample : samples) {
        int ok = 0;
        for (int testop = 0; testop < 16; ++testop) {
            Op test = sample.op;
            test.opcode = testop;
            auto regs = sample.before;
            Run(test, regs);
            if (regs != sample.after) {
                possibles[sample.op.opcode].erase(testop);
            }
            else {
                ++ok;
            }
        }
        if (ok >= 3) {
            ++part1;
        }
    }

    cout << "Part 1 = " << part1 << endl;

    OpMap opmap;
    while (opmap.size() < 16) {
        for (auto& e : possibles) {
            if (e.second.size() == 1) {
                int val = *e.second.begin();
                opmap[e.first] = *e.second.begin();

                possibles.erase(e.first);
                for (auto& x : possibles) {
                    x.second.erase(val);
                }
                break;
            }
        }
    }

    vector<int> regs = { 0,0,0,0 };
    while (!cin.eof()) {
        string s;
        getline(cin, s);
        if (s.empty()) continue;

        smatch sm;
        regex_match(s.cbegin(), s.cend(), sm, INPUT_R);

        Op op;
        op.opcode = stoi(sm[1]);
        op.src1 = stoi(sm[2]);
        op.src2 = stoi(sm[3]);
        op.dest = stoi(sm[4]);

        Run(opmap, op, regs);
        //cout << "[" << regs[0] << ", " << regs[1] << ", " << regs[2] << ", " << regs[3] << "]" << endl;
    }

    cout << "Part 2 = " << regs[0] << endl;
}
