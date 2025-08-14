#include <iostream>  
#include <thread>
#include <semaphore>
#include <chrono>

#include "parser.h"
#include "memory.h"
#include "thread.h"

using namespace std;

constexpr int MEM_SIZE = 1024;
vector<int> memory(MEM_SIZE, 0);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename> <num_threads>" << endl;
        return 1;
    }

    const std::string filename = argv[1];
    const std::vector<instruction_t> instructions = parser(filename);

    if (instructions.empty()) {
        std::cout << "No instructions found or file could not be opened." << std::endl;
        return 0;
    }
    //print out the parsed instructions

    printInstrs(instructions);

    //initialize threads, give each thread: instructions vector, thread state 

    int num_threads = std::stoi(argv[2]);

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(thread_func, i, instructions);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}