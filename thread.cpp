#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <cctype>
#include <semaphore>

#include "parser.h"
#include "memory.h"
#include "thread.h"


std::counting_semaphore<1> cout_sem(1);

ArgType getArgType(const std::string &arg) {
    if (arg.empty()) return REG; 
    if (arg[0] == '#' && arg.size() > 1 && std::isdigit(arg[1])) {
        return IMM; 
    }
    if ( arg[0] == 'R' && arg.size() > 1 && std::isdigit(arg[1])) {
        return REG; 
    }   //assumes kernel file is correct
}

int parseArg(const std::string &arg) {
    return std::stoi(arg.substr(1)); 
}

//opcodes to execute: ADD, SUB, MUL, DIV,  MOV, LOAD, STORE, HALT, LOADID

int execute(ThreadState& thread, std::vector<instruction_t> instructions){
    for (const auto& instr : instructions){
        if (std::strcmp(instr.opcode, "ADD") == 0) {
            thread.pc++;
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
        }else if (std::strcmp(instr.opcode,  "SUB") == 0){
            thread.pc++;
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
        }else if(std::strcmp(instr.opcode, "MUL") == 0){
            thread.pc++;
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

        }else if (std::strcmp(instr.opcode, "DIV") == 0){
            thread.pc++;
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
        }else if (std::strcmp(instr.opcode, "MOV") == 0){
            thread.pc++;
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = src;
        }else if (std::strcmp(instr.opcode, "STORE") == 0){
            thread.pc++;
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            memory[thread.reg[dest]] = src; //not safe bc out of bounds is possible
        }else if (std::strcmp(instr.opcode, "LOAD") == 0){
            thread.pc++;
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = memory[src];
        }else if (std::strcmp(instr.opcode, "HALT") == 0){



            cout_sem.acquire(); 
            std::cout << "Thread " << thread.tid << " finished execution." << std::endl;
            cout_sem.release();

            return 0;
        }else if (std::strcmp(instr.opcode, "LOADID") == 0){
            thread.pc++;
            int dest = parseArg(instr.args[0]);
            thread.reg[dest] = thread.tid; 
        }
    }
    
    return 1; //instructions end with no  halt message
}


int thread_func(int thread_id, std::vector<instruction_t> instructions){

    ThreadState thread;
    thread.tid = thread_id;
    thread.pc = 0;

    int ex = execute(thread, instructions);

    return ex;
}