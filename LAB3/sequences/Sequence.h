//
//  Sequence.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef Sequence_h
#define Sequence_h

#include <iostream>
using namespace std;

template <class T>
class Sequence {
public:
    virtual T & Get(size_t index) = 0;
    virtual const T & Get(size_t index) const = 0;
    virtual T & GetFirst() const = 0;
    virtual T & GetLast() const = 0;
    virtual size_t GetLength() const = 0;
    virtual void Set(size_t index, const T & value) = 0;
    virtual void Append(const T & value) = 0;
    virtual ~Sequence(){};
};

template <class T>
void Print(const Sequence<T> & other) {
    cout << "{" << other.Get(0);
    for (int i = 1; i < other.GetLength(); i++) {
        cout << ", " << other.Get(i);
    }
    cout << "}" << endl;
}
    


#endif /* Sequence_h */
