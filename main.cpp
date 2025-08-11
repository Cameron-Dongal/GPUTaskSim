#include <iostream>  
#include <thread>
#include <semaphore>
#include <chrono>

#include "parser.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    const std::string filename = argv[1];
    std::vector<instruction_t> instructions = parser(filename);

    if (instructions.empty()) {
        std::cout << "No instructions found or file could not be opened." << std::endl;
        return 0;
    }

    for (const auto& instr : instructions) {
        std::cout << "Opcode: " << instr.opcode << ", Args: ";
        for (int i = 0; i < instr.num_args; ++i) {
            std::cout << instr.args[i] << (i < instr.num_args - 1 ? ", " : "");
        }
        std::cout << std::endl;
    }

    /*

    // Example of using a semaphore

    counting_semaphore<1> sem(1); // Create a semaphore with an initial count of 1
    sem.acquire(); // Acquire the semaphore
    cout << "Semaphore acquired!" << endl;
    
    // Simulate some work
    this_thread::sleep_for(std::chrono::seconds(1));
    
    sem.release(); // Release the semaphore
    cout << "Semaphore released!" << endl;

    */
    return 0;
}