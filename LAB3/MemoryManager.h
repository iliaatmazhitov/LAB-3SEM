//
// Created by Илья Атмажитов on 16.01.2025.
//

#ifndef LABORATORY3_MEMORYMANAGER_H
#define LABORATORY3_MEMORYMANAGER_H

#include <map>

using namespace std;

class MemoryManager {
private:
    map<uint8_t*, size_t> _tree;
    uint8_t* _memory;
    size_t _capacity;
public:
    MemoryManager(size_t size);
    ~MemoryManager();

    void* allocate(size_t size);
    void deallocate(void* ptr);

};

#endif //LABORATORY3_MEMORYMANAGER_H
