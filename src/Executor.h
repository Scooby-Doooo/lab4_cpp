#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "ThreadSafeData.h"
#include <string>
#include <vector>
#include <chrono>

// Handles command execution from files
class Executor {
public:
    struct Command {
        enum Type { READ, WRITE, STRING };
        Type type;
        int fieldIndex;
        int value;
    };

    // Load commands from file (time not measured)
    static std::vector<Command> loadCommands(const std::string& filename);

    // Execute commands and measure time (excludes file loading)
    static double executeCommands(const std::vector<Command>& commands, 
                                  ThreadSafeData& data);

    // Single-threaded execution from one file
    static double executeSingleThread(const std::string& filename, 
                                       ThreadSafeData& data);

    // Multi-threaded execution, each thread processes one file
    static double executeMultiThread(const std::vector<std::string>& filenames, 
                                      ThreadSafeData& data);

private:
    static Command parseCommand(const std::string& line);
};

#endif // EXECUTOR_H
