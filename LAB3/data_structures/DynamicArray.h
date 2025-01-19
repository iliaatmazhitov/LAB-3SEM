//
//  DynamicArray.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef DynamicArray_h
#define DynamicArray_h

#include <stdexcept>


template<class T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;
public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(size_t count) {
        data = new T[count];
        size = count;
        capacity = count;
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) {
        size = dynamicArray.size;
        capacity = dynamicArray.capacity;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = dynamicArray.data[i];
        }
    }

    ~DynamicArray() {
        Clear();
    }
    
    void Clear() {
        delete[] data;
    }

    T& Get(size_t index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    size_t GetSize() const{
        return size;
    }
    
    T & GetFirst() const{
        if (size == 0) {
            throw std::out_of_range("Array is empty");
        }

        return data[0];
    }

    T & GetLast() const{
        if (size == 0) {
            throw std::out_of_range("Array is empty");
        }

        return data[size - 1];
    }


    void Set(size_t index, const T & value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        data[index] = value;
    }


    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }


    void Append(const T & value) {
        if (size == capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size] = value;
        size++;
    }

};


#endif /* DynamicArray_h */
