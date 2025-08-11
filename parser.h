#pragma once
#include <string>
#include <vector>

typedef struct {
    char opcode[16];
    char args[3][16];
    int num_args;
} instruction_t;

std::vector<instruction_t> parser(const std::string filename);