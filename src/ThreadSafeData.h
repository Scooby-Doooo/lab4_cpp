#ifndef THREAD_SAFE_DATA_H
#define THREAD_SAFE_DATA_H

#include <mutex>
#include <string>

// Thread-safe data structure with 3 integer fields
// Uses separate mutexes per field for better concurrency
// get/set lock only their field's mutex, string op locks all 3
class ThreadSafeData {
private:
    static constexpr int NUM_FIELDS = 3;
    
    int fields[NUM_FIELDS];
    mutable std::mutex mutexes[NUM_FIELDS];

public:
    ThreadSafeData();

    void set(int fieldIndex, int value);
    int get(int fieldIndex) const;
    
    // Locks all mutexes to get consistent snapshot
    operator std::string() const;
};

#endif // THREAD_SAFE_DATA_H
