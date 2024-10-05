//
//  SmartPointer.h
//  laboratory1
//
//  Created by Илья Атмажитов on 24.09.2024.
//

#ifndef SmartPointer_h
#define SmartPointer_h

#include <stdexcept>

using namespace std;

struct Counters {
    size_t smrt;
    size_t weak;
};

template <class T>
class WeakPtr;

template <class T>
class SmrtPtr {
private:
    T* data;
    Counters* counter;
    
    void Unconnect() {
        if (counter == nullptr) {
            return;
        }
        
        counter->smrt--;
        
        if (counter->smrt == 0) {
            delete data;
            if (counter -> weak == 0) {
                delete counter;
            }
        }
    }
public:
    SmrtPtr(T* value = nullptr) : data(value), counter(!value ? nullptr : new Counters{1, 0}) {}
    
    SmrtPtr(const SmrtPtr & other): data(other.data), counter(other.counter) {
        if (counter == nullptr) {
            return;
        }
        
        counter -> smrt++;
    }
    
    ~SmrtPtr() {
        Unconnect();
    }
    
    SmrtPtr(SmrtPtr && other): data(other.data), counter(other.counter) {
        other.data = nullptr;
        other.counter = nullptr;
    }
    
    SmrtPtr & operator=(SmrtPtr && other) {
        if (this == &other) {
            return *this;
        }
        
        Unconnect();
        data = other.data;
        counter = other.counter;
        
        other.data = nullptr;
        other.counter = nullptr;
        
        return *this;
    }
    
    SmrtPtr & operator=(const SmrtPtr & other) {
        if (this == &other) {
            return *this;
        }
        
        Unconnect();
        data = other.data;
        counter = other.counter;
        
        if (counter != nullptr) {
            counter->smrt++;
        }
        
        return *this;
    }
    
    
    T* Get() const {
        return data;
    }
    
    size_t GetRefCount() const {
        return (counter != nullptr) ? counter->smrt : 0;
    }

    
    T & operator*() const  {
        return *data;
    }
    
    T* operator->() const {
        return data;
    }
    
    operator bool() const {
        return data != nullptr;
    }
    
    bool operator == (const SmrtPtr & other) const{
        return data == other.data;
    }
    
    bool operator != (const SmrtPtr & other) const{
        return data != other.data;
    }
    
    template <typename U>
    friend class WeakPtr;
};

template <class T>
class WeakPtr {
private:
    T* data;
    Counters* counter;
    
    void Unconnect() {
        if (counter != nullptr) {
            counter->weak--;
            if (counter->smrt == 0 && counter->weak == 0) {
                delete counter;
            }
        }
    }
public:
    WeakPtr(const SmrtPtr<T> & other): data(other.data), counter(other.counter) {
        if (counter != nullptr) {
            counter->weak++;
        }
    }
    
    WeakPtr(const WeakPtr & other): data(other.data), counter(other.counter) {
        if (counter != nullptr) {
            counter->weak++;
        }
    }
    
    WeakPtr(WeakPtr && other): data(other.data), counter(other.counter) {
        other.counter = nullptr;
        other.data = nullptr;
    }
    
    WeakPtr & operator=(const WeakPtr & other) {
        if (this == &other) {
            return *this;
        }
        
        Unconnect();
        data = other.data;
        counter = other.counter;
        
        if (counter != nullptr) {
            counter->weak++;
        }
        
        return *this;
    }
        
    WeakPtr & operator=(WeakPtr && other) {
        if (this == &other) {
            return *this;
        }
        
        Unconnect();
        data = other.data;
        counter = other.counter;
        
        other.data = nullptr;
        other.counter = nullptr;
        
        return *this;
    }
    
    
    size_t GetRefCount() const {
            return (counter != nullptr) ? counter->weak : 0;
    }
    
    bool Expired() const {
        if (counter == nullptr) {
            return true;
        } else if (counter->smrt == 0) {
            return true;
        }
        return false;
    }

    SmrtPtr<T> Lock() const {
        if (Expired()) {
            return nullptr;
        }
        SmrtPtr<T> res;
        res.data = data;
        res.counter = counter;
        
        if (counter != nullptr) {
            counter->smrt++;
        }
        
        return res;
    }
    
    T & operator*() const  {
        return *data;
    }
    
    T* operator->() const {
        return data;
    }
    
    operator bool() const {
        return data != nullptr;
    }
    
    bool operator == (const WeakPtr & other) const{
        return data == other.data;
    }
    
    bool operator != (const WeakPtr & other) const{
        return data != other.data;
    }
    
};

#endif /* SmartPointer_h */
