#include "ThreadSafeData.h"
#include "FileGenerator.h"
#include "Executor.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

const int NUM_OPERATIONS = 100000;

// Run a test scenario with specified files and thread count
void runTest(const std::string& testName, 
             const std::vector<std::string>& filenames, 
             int numThreads) {
    
    std::cout << "\n  " << testName << " (" << numThreads << " thread(s)):" << std::endl;
    
    ThreadSafeData data;
    double executionTime;
    
    if (numThreads == 1) {
        executionTime = Executor::executeSingleThread(filenames[0], data);
    } else {
        executionTime = Executor::executeMultiThread(filenames, data);
    }
    
    std::cout << "  Execution time: " << std::fixed << std::setprecision(2) 
              << executionTime << " ms" << std::endl;
}

int main() {
    std::cout << "Lab 4: Thread-Safe Data Structure (Variant 10)" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    try {
        FileGenerator::generateVariant10File("commands_a.txt", NUM_OPERATIONS);
        FileGenerator::generateEqualFrequenciesFile("commands_b.txt", NUM_OPERATIONS);
        FileGenerator::generateCustomFile("commands_c.txt", NUM_OPERATIONS);
        
        std::cout << "Test Set A: Variant 10 frequencies" << std::endl;
        runTest("File A", {"commands_a.txt"}, 1);
        
        FileGenerator::generateVariant10File("commands_a_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateVariant10File("commands_a_t2.txt", NUM_OPERATIONS);
        runTest("File A", {"commands_a_t1.txt", "commands_a_t2.txt"}, 2);
        
        FileGenerator::generateVariant10File("commands_a_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateVariant10File("commands_a_t2.txt", NUM_OPERATIONS);
        FileGenerator::generateVariant10File("commands_a_t3.txt", NUM_OPERATIONS);
        runTest("File A", {"commands_a_t1.txt", "commands_a_t2.txt", "commands_a_t3.txt"}, 3);
        
        std::cout << "\nTest Set B: Equal frequencies" << std::endl;
        runTest("File B", {"commands_b.txt"}, 1);
        
        FileGenerator::generateEqualFrequenciesFile("commands_b_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateEqualFrequenciesFile("commands_b_t2.txt", NUM_OPERATIONS);
        runTest("File B", {"commands_b_t1.txt", "commands_b_t2.txt"}, 2);
        
        FileGenerator::generateEqualFrequenciesFile("commands_b_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateEqualFrequenciesFile("commands_b_t2.txt", NUM_OPERATIONS);
        FileGenerator::generateEqualFrequenciesFile("commands_b_t3.txt", NUM_OPERATIONS);
        runTest("File B", {"commands_b_t1.txt", "commands_b_t2.txt", "commands_b_t3.txt"}, 3);
        
        std::cout << "\nTest Set C: Custom frequencies" << std::endl;
        runTest("File C", {"commands_c.txt"}, 1);
        
        FileGenerator::generateCustomFile("commands_c_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateCustomFile("commands_c_t2.txt", NUM_OPERATIONS);
        runTest("File C", {"commands_c_t1.txt", "commands_c_t2.txt"}, 2);
        
        FileGenerator::generateCustomFile("commands_c_t1.txt", NUM_OPERATIONS);
        FileGenerator::generateCustomFile("commands_c_t2.txt", NUM_OPERATIONS);
        FileGenerator::generateCustomFile("commands_c_t3.txt", NUM_OPERATIONS);
        runTest("File C", {"commands_c_t1.txt", "commands_c_t2.txt", "commands_c_t3.txt"}, 3);
        
    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
