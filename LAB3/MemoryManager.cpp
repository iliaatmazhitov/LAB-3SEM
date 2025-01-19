//
// Created by Илья Атмажитов on 16.01.2025.
//

#include <iterator>
#include "MemoryManager.h"

MemoryManager::MemoryManager(size_t size): _memory(new uint8_t[size]), _capacity(size) {}

MemoryManager::~MemoryManager() {
    delete[] _memory;
}

void *MemoryManager::allocate(size_t size) {
    if (_tree.empty()) {
        if (size > _capacity) {
            return nullptr;
        }
        _tree[_memory] = size;
        return _memory;
    }

    auto it = _tree.begin();
    if (it -> first - _memory >= size) {
        _tree[_memory] = size;
        return _memory;
    }

    auto next = std::next(it);
    while (next != _tree.end()) {
        if (next->first - (it->first + it->second) >= size) {
            _tree[it->first + it->second] = size;
            return it->first + it->second;
        }
        it = next;
        next = std::next(next);
    }

    size_t sum = it->first - _memory;

    if (_capacity - (sum + it->second) >= size) {
        _tree[it->first + it->second] = size;
        return it->first + it->second;
    }

    return nullptr;
}

void MemoryManager::deallocate(void *ptr) {
    auto uint_ptr = reinterpret_cast<uint8_t*>(ptr);
    _tree.erase(uint_ptr);
}

