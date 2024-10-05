//
//  DynamicArray.h
//  laboratory1
//
//  Created by Илья Атмажитов on 26.09.2024.
//

#ifndef DynamicArray_h
#define DynamicArray_h

#include <stdexcept>
#include "SmartPointer.h"


template<class T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;
public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}

    DynamicArray(const DynamicArray<T>& dynamicArray) {
        size = dynamicArray.size;
        capacity = dynamicArray.capacity;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = dynamicArray.data[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    T Get(size_t index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    size_t GetSize() const{
        return size;
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

//template <class T>
//void Print(const DynamicArray<SmrtPtr<T>>& array) {
//    if (array.GetSize() == 0) {
//        cout << "{}" << endl;
//        return;
//    }
//
//    cout << "{";
//    if (array.Get(0)) {
//        cout << *array.Get(0);
//    } else {
//        cout << "null";
//    }
//
//    for (size_t i = 1; i < array.GetSize(); i++) {
//        cout << " -> ";
//        if (array.Get(i)) {
//            cout << *array.Get(i) << "; " << array.Get(i).GetRefCount();
//        } else {
//            cout << "null";
//        }
//    }
//    cout << "}" << endl;
//}

#endif /* DynamicArray_h */
