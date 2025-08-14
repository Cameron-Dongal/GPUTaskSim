#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <cctype>

#include "parser.h"
#include "memory.h"
#include "thread.h"

ArgType getArgType(const std::string &arg) {
    if (arg.empty()) return REG; 
    if (arg[0] == '#' && arg.size() > 1 && std::isdigit(arg[1])) {
        return IMM; 
    }
    if ( arg[0] == 'R' && arg.size() > 1 && std::isdigit(arg[1])) {
        return REG; 
    }   
}

int parseArg(const std::string &arg) {
    return std::stoi(arg.substr(1)); 
}

//opcodes to execute: ADD, SUB, MUL, DIV,  MOV, LOAD, STORE, HALT, LOADID

int execute(ThreadState& thread, std::vector<instruction_t> instructions){
    for (const auto& instr : instructions){
        if (instr.opcode == "ADD"){
            int dest = parseArg(instr.args[0]);
            int src1;
            if (getArgType(instr.args[1]) == IMM) {
                src1 = parseArg(instr.args[1]);
            } else {
                src1 = thread.reg[parseArg(instr.args[1])];
            }
            int src2;
            if (getArgType(instr.args[2]) == IMM) {
                src2 = parseArg(instr.args[2]);
            } else {
                src2 = thread.reg[parseArg(instr.args[2])];
            }
            thread.reg[dest] = src1 + src2;
        }else if (instr.opcode ==  "SUB"){
            int dest = parseArg(instr.args[0]);
            int src1;
            if (getArgType(instr.args[1]) == IMM) {
                src1 = parseArg(instr.args[1]);
            } else {
                src1 = thread.reg[parseArg(instr.args[1])];
            }
            int src2;
            if (getArgType(instr.args[2]) == IMM) {
                src2 = parseArg(instr.args[2]);
            } else {
                src2 = thread.reg[parseArg(instr.args[2])];
            }
            thread.reg[dest] = src1 - src2;
        }else if(instr.opcode == "MUL"){
            int dest = parseArg(instr.args[0]);
            int src1;
            if (getArgType(instr.args[1]) == IMM) {
                src1 = parseArg(instr.args[1]);
            } else {
                src1 = thread.reg[parseArg(instr.args[1])];
            }
            int src2;
            if (getArgType(instr.args[2]) == IMM) {
                src2 = parseArg(instr.args[2]);
            } else {
                src2 = thread.reg[parseArg(instr.args[2])];
            }
            thread.reg[dest] = src1 * src2;

        }else if (instr.opcode == "DIV"){
            int dest = parseArg(instr.args[0]);
            int src1;
            if (getArgType(instr.args[1]) == IMM) {
                src1 = parseArg(instr.args[1]);
            } else {
                src1 = thread.reg[parseArg(instr.args[1])];
            }
            int src2;
            if (getArgType(instr.args[2]) == IMM) {
                src2 = parseArg(instr.args[2]);
            } else {
                src2 = thread.reg[parseArg(instr.args[2])];
            }
            thread.reg[dest] = src1 / src2;
        }else if (instr.opcode == "MOV"){
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = src;
        }else if (instr.opcode == "STORE"){
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            memory[thread.reg[dest]] = src; //not safe bc out of bounds is possible
        }else if (instr.opcode == "LOAD"){
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = memory[src];
        }else if (instr.opcode == "HALT"){
            return 0;
        }else if (instr.opcode == "LOADID"){
            int dest = parseArg(instr.args[0]);
            thread.reg[dest] = thread.tid; 
        }
    }
    return 1; //instructions end with no  halt message
}


