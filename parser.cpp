#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <cstring> // for strncpy
#include "parser.h"

//does  not  check if instructions are valid

std::vector<instruction_t> parser(const std::string filename) {

    std::ifstream file(filename);
    if (!file) {
        return {};
    }

    std::string line;
    std::vector<instruction_t> instructions; //vector   to hold instructions

    while (std::getline(file, line)) {
        size_t comment = line.find(';'); //find comment in a line
        if (comment != std::string::npos) { //if comment found
           line = line.substr(0, comment);  //save only the non-comment part of line
           if (line.empty()) { //if noncomment part is empty then continue to next line
                continue;
            }
        }
        
        std::istringstream iss(line);
        std::string token;
        instruction_t instruction = {};
        int arg_index = 0;

        if (iss >> token) {
            strncpy(instruction.opcode, token.c_str(), sizeof(instruction.opcode) - 1);
        }else{
            continue; //no opcode for some reason but  should never happen
        }

        while  (iss >> token && arg_index < 3) { //read arguments
            strncpy(instruction.args[arg_index], token.c_str(), sizeof(instruction.args[arg_index]) - 1);
            arg_index++;
        }
        instruction.num_args = arg_index; //num of args  in the current instruction
        instructions.push_back(instruction); //add instruction to the vector

    }

    file.close();
    return instructions; //return vector of instructions
}

void printInstrs(const std::vector<instruction_t>& instructions) {
    for (const auto& instr : instructions) {
        std::cout << "Opcode: " << instr.opcode << ", Args: ";
        for (int i = 0; i < instr.num_args; ++i) {
            std::cout << instr.args[i] << (i < instr.num_args - 1 ? ", " : "");
        }
        std::cout << std::endl;
    }
}

