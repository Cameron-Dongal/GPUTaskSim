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
    while(thread.pc < instructions.size()){
        const auto& instr = instructions[thread.pc];
        if (std::strcmp(instr.opcode, "ADD") == 0) {
            
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
            if (thread.tid == 0){
                std::cout << "ADD performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode,  "SUB") == 0){
            
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
            if (thread.tid == 0){
                std::cout << "SUB performed, PC = " << thread.pc << std::endl;
            }
        }else if(std::strcmp(instr.opcode, "MUL") == 0){
            
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
            if (thread.tid == 0){
                std::cout << "MUL performed, PC = " << thread.pc << std::endl;
            }

        }else if (std::strcmp(instr.opcode, "DIV") == 0){
            
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
                        if (thread.tid == 0){
                std::cout << "DIV performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode, "MOV") == 0){
            
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = src;
            if (thread.tid == 0){
                std::cout << "MOV performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode, "STORE") == 0){
            
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            memory[thread.reg[dest]] = src; //not safe bc out of bounds is possible
            if (thread.tid == 0){
                std::cout << "STORE performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode, "LOAD") == 0){
            
            int dest = parseArg(instr.args[0]);
            int src;
            if (getArgType(instr.args[1]) == IMM) {
                src = parseArg(instr.args[1]);
            } else {
                src = thread.reg[parseArg(instr.args[1])];
            }
            thread.reg[dest] = memory[src];
            if (thread.tid == 0){
                std::cout << "LOAD performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode, "HALT") == 0){



            cout_sem.acquire(); 
            std::cout << "Thread " << thread.tid << " finished execution." << std::endl;
            cout_sem.release();

            return 0;
        }else if (std::strcmp(instr.opcode, "LOADID") == 0){
            
            int dest = parseArg(instr.args[0]);
            thread.reg[dest] = thread.tid; 
            if (thread.tid == 0){
                std::cout << "LOADID performed, PC = " << thread.pc << std::endl;
            }
        }else if (std::strcmp(instr.opcode, "JUMP") == 0){

            if (thread.tid == 0){
                std::cout << "JUMP performed, PC = " << thread.pc << std::endl;
            }
            thread.pc = parseArg(instr.args[0]);

            continue;
        }else if (std::strcmp(instr.opcode, "BEQ") == 0){ 
            int src1 = (getArgType(instr.args[0]) == IMM) ? parseArg(instr.args[0]) : thread.reg[parseArg(instr.args[0])];
            int src2 = (getArgType(instr.args[1]) == IMM) ? parseArg(instr.args[1]) : thread.reg[parseArg(instr.args[1])];
            if (src1 == src2) {
                if (thread.tid == 0){
                    std::cout << "BEQ success, PC = " << thread.pc << std::endl;
                }
                thread.pc = parseArg(instr.args[2]);
                continue;
            }
            

            if (thread.tid == 0){
                std::cout << "BEQ fail, PC = " << thread.pc << std::endl;
            }
            
        }
        thread.pc++;
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