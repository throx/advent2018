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

    int ip = 0;
    vector<int> regs;
    regs.resize(6);

    while (ip >= 0 && ip < prog.size()) {
        regs[bound_ip] = ip;
        Run(prog[ip], regs);
        ip = regs[bound_ip];
        ++ip;
    }

    cout << "Part 1: " << regs[0] << endl;
/*
*   Don't need to run the prog - have to reverse engineer
*   refer "annotated.txt"
*
    ip = 0;
    regs = vector<int>{ 1, 0, 0, 0, 0, 0 };

    while (ip >= 0 && ip < prog.size()) {
        regs[bound_ip] = ip;
        cout << "IP: " << ip;
        PrintRegs(regs);
        PrintOp(prog[ip]);
        Run(prog[ip], regs);
        PrintRegs(regs);
        cout << endl;
        ip = regs[bound_ip];
        ++ip;
    }
*/
    int num = 10551277;
    int sum = 0;
    for (int i = 1; i <= 10551277; ++i) {
        if (num % i == 0) sum += i;
    }
    cout << "Part 2: " << sum << endl;

}
