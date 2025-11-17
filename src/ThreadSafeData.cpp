#include "ThreadSafeData.h"
#include <sstream>
#include <stdexcept>

ThreadSafeData::ThreadSafeData() {
    for (int i = 0; i < NUM_FIELDS; ++i) {
        fields[i] = 0;
    }
}

void ThreadSafeData::set(int fieldIndex, int value) {
    if (fieldIndex < 0 || fieldIndex >= NUM_FIELDS) {
        throw std::out_of_range("Field index out of range");
    }
    
    std::lock_guard<std::mutex> lock(mutexes[fieldIndex]);
    fields[fieldIndex] = value;
}

int ThreadSafeData::get(int fieldIndex) const {
    if (fieldIndex < 0 || fieldIndex >= NUM_FIELDS) {
        throw std::out_of_range("Field index out of range");
    }
    
    std::lock_guard<std::mutex> lock(mutexes[fieldIndex]);
    return fields[fieldIndex];
}

ThreadSafeData::operator std::string() const {
    int tempValues[NUM_FIELDS];
    
    // Lock all mutexes atomically to avoid deadlock
    std::lock(mutexes[0], mutexes[1], mutexes[2]);
    
    std::lock_guard<std::mutex> lock0(mutexes[0], std::adopt_lock);
    std::lock_guard<std::mutex> lock1(mutexes[1], std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mutexes[2], std::adopt_lock);
    
    for (int i = 0; i < NUM_FIELDS; ++i) {
        tempValues[i] = fields[i];
    }
    
    std::ostringstream oss;
    oss << "[" << tempValues[0] << ", " << tempValues[1] << ", " << tempValues[2] << "]";
    return oss.str();
}
