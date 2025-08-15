#pragma once
#include <string>
#include <vector>
#include <cctype>
#include "parser.h"
constexpr int NUM_REGS = 32;

struct ThreadState {
    int reg[NUM_REGS] = {0};
    int tid;
    int pc;
};

enum ArgType { REG, IMM };

ArgType getArgType(const std::string &arg);
int parseArg(const std::string &arg);
int execute(ThreadState& thread, std::vector<instruction_t> instructions);
int thread_func(int thread_id, std::vector<instruction_t> instructions);