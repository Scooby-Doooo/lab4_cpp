#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <string>
#include <vector>

// Generates test files with operation sequences
class FileGenerator {
public:
    struct OperationFrequencies {
        double writeField0;
        double writeField1;
        double writeField2;
        double readField0;
        double readField1;
        double readField2;
        double stringOp;
        
        OperationFrequencies() 
            : writeField0(0), writeField1(0), writeField2(0),
              readField0(0), readField1(0), readField2(0),
              stringOp(0) {}
    };

    // Type A: variant 10 frequencies (write0: 40%, string: 47%, etc.)
    static void generateVariant10File(const std::string& filename, int numOperations);

    // Type B: equal frequencies (~14.3% each)
    static void generateEqualFrequenciesFile(const std::string& filename, int numOperations);

    // Type C: custom frequencies (read-heavy workload)
    static void generateCustomFile(const std::string& filename, int numOperations);

    // Generic generator with custom frequencies (must sum to 100)
    static void generateFile(const std::string& filename, 
                            int numOperations, 
                            const OperationFrequencies& frequencies);

private:
    static std::string generateRandomOperation(const OperationFrequencies& frequencies);
};

#endif // FILE_GENERATOR_H
