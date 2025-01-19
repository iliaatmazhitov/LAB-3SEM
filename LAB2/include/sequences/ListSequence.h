//
//  ListSequence.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef ListSequence_h
#define ListSequence_h

#include "Sequence.h"
#include "../data_structures/LinkedList.h"
#include <stdexcept>

using namespace std;

template <class T> 
class ListSequence : public Sequence<T> {
private:
    LinkedList<T>* items;
public:
    ListSequence() {
        this->items = new LinkedList<T>;
    }
    
    ListSequence(T* items, int count) {
        this->items = new LinkedList<T>(items, count);
    }
    
    
    ~ListSequence() {
        this->items->Clear();
    }
    
    size_t GetLength() const{
        return this->items->GetLength();
    }
    
    void Append(const T & value) {
        this->items->Append(value);
    }
    
    T & Get(size_t index) {
        return this->items->Get(index);
    }

    const T & Get(size_t index) const {
        return this->items->Get(index);
    }

    T & GetLast() const {
        return this->items->GetLast();
    }
    
    T & GetFirst() const{
        return this->items->GetFirst();
    }
    
    void Set(size_t index, const T & value) {
        this->items->Set(index, value);
    }
    
    ListSequence(const ListSequence<T>& other) {
        this->items = new LinkedList<T>(*other.items);
        if (this->items == nullptr) {
            throw logic_error("Failed to allocate memory");
        }
    }
    
    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this != &other) {
            delete this->items;
            this->items = new LinkedList<T>(*other.items);
            if (this->items == nullptr) {
                throw logic_error("Failed to allocate memory");
            }
        }
        return *this;
    }
};

#endif /* ListSequence_h */
