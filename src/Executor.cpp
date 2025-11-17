#include "Executor.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <thread>

std::vector<Executor::Command> Executor::loadCommands(const std::string& filename) {
    std::vector<Command> commands;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        commands.push_back(parseCommand(line));
    }
    
    file.close();
    return commands;
}

double Executor::executeCommands(const std::vector<Command>& commands, ThreadSafeData& data) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for (const auto& cmd : commands) {
        switch (cmd.type) {
            case Command::READ:
                data.get(cmd.fieldIndex);
                break;
                
            case Command::WRITE:
                data.set(cmd.fieldIndex, cmd.value);
                break;
                
            case Command::STRING: {
                std::string result = static_cast<std::string>(data);
                (void)result;  // prevent optimization
                break;
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    return duration.count();
}

double Executor::executeSingleThread(const std::string& filename, ThreadSafeData& data) {
    std::vector<Command> commands = loadCommands(filename);
    return executeCommands(commands, data);
}

double Executor::executeMultiThread(const std::vector<std::string>& filenames, ThreadSafeData& data) {
    std::vector<std::vector<Command>> allCommands;
    
    for (const auto& filename : filenames) {
        std::vector<Command> commands = loadCommands(filename);
        allCommands.push_back(std::move(commands));
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    
    for (size_t i = 0; i < allCommands.size(); ++i) {
        threads.emplace_back([&allCommands, &data, i]() {
            for (const auto& cmd : allCommands[i]) {
                switch (cmd.type) {
                    case Command::READ:
                        data.get(cmd.fieldIndex);
                        break;
                        
                    case Command::WRITE:
                        data.set(cmd.fieldIndex, cmd.value);
                        break;
                        
                    case Command::STRING: {
                        std::string result = static_cast<std::string>(data);
                        (void)result;
                        break;
                    }
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    return duration.count();
}

Executor::Command Executor::parseCommand(const std::string& line) {
    Command cmd;
    std::istringstream iss(line);
    std::string operation;
    iss >> operation;
    
    if (operation == "read") {
        cmd.type = Command::READ;
        iss >> cmd.fieldIndex;
        cmd.value = 0;
    } 
    else if (operation == "write") {
        cmd.type = Command::WRITE;
        iss >> cmd.fieldIndex >> cmd.value;
    } 
    else if (operation == "string") {
        cmd.type = Command::STRING;
        cmd.fieldIndex = 0;
        cmd.value = 0;
    } 
    else {
        throw std::runtime_error("Unknown operation: " + operation);
    }
    
    return cmd;
}
