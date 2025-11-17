#include "FileGenerator.h"
#include <fstream>
#include <random>
#include <stdexcept>
#include <iostream>

void FileGenerator::generateVariant10File(const std::string& filename, int numOperations) {
    OperationFrequencies freq;
    freq.writeField0 = 40.0;
    freq.writeField1 = 5.0;
    freq.writeField2 = 5.0;
    freq.readField0 = 1.0;
    freq.readField1 = 1.0;
    freq.readField2 = 1.0;
    freq.stringOp = 47.0;
    
    generateFile(filename, numOperations, freq);
}

void FileGenerator::generateEqualFrequenciesFile(const std::string& filename, int numOperations) {
    OperationFrequencies freq;
    double equalFreq = 100.0 / 7.0;
    freq.writeField0 = equalFreq;
    freq.writeField1 = equalFreq;
    freq.writeField2 = equalFreq;
    freq.readField0 = equalFreq;
    freq.readField1 = equalFreq;
    freq.readField2 = equalFreq;
    freq.stringOp = equalFreq;
    
    generateFile(filename, numOperations, freq);
}

void FileGenerator::generateCustomFile(const std::string& filename, int numOperations) {
    OperationFrequencies freq;
    freq.writeField0 = 10.0;
    freq.writeField1 = 10.0;
    freq.writeField2 = 10.0;
    freq.readField0 = 20.0;
    freq.readField1 = 20.0;
    freq.readField2 = 20.0;
    freq.stringOp = 10.0;
    
    generateFile(filename, numOperations, freq);
}

void FileGenerator::generateFile(const std::string& filename, 
                                 int numOperations, 
                                 const OperationFrequencies& frequencies) {
    double sum = frequencies.writeField0 + frequencies.writeField1 + frequencies.writeField2 +
                 frequencies.readField0 + frequencies.readField1 + frequencies.readField2 +
                 frequencies.stringOp;
    
    if (std::abs(sum - 100.0) > 0.01) {
        throw std::invalid_argument("Sum of frequencies must equal 100");
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    for (int i = 0; i < numOperations; ++i) {
        std::string operation = generateRandomOperation(frequencies);
        file << operation << "\n";
    }
    
    file.close();
}

std::string FileGenerator::generateRandomOperation(const OperationFrequencies& frequencies) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 100.0);
    
    double randValue = dis(gen);
    double cumulative = 0.0;
    
    cumulative += frequencies.writeField0;
    if (randValue < cumulative) {
        return "write 0 1";
    }
    
    cumulative += frequencies.writeField1;
    if (randValue < cumulative) {
        return "write 1 1";
    }
    
    cumulative += frequencies.writeField2;
    if (randValue < cumulative) {
        return "write 2 1";
    }
    
    cumulative += frequencies.readField0;
    if (randValue < cumulative) {
        return "read 0";
    }
    
    cumulative += frequencies.readField1;
    if (randValue < cumulative) {
        return "read 1";
    }
    
    cumulative += frequencies.readField2;
    if (randValue < cumulative) {
        return "read 2";
    }
    
    return "string";
}
