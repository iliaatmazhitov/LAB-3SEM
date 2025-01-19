//
//  Comparators.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef Comparators_h
#define Comparators_h
#include <string>

template <class T>
class IComp {
public:
    virtual bool operator()(const T & first, const T & second) const = 0;
};

template <class T>
class Less: public IComp<T> {
public:
    Less() {}

    bool operator()(const T & first, const T & second) const override {
        return first < second;
    }
};

template <class T>
class Greater: public IComp<T> {
public:
    Greater() {}

    bool operator()(const T & first, const T & second) const override {
        return first > second;
    }
};

class StringCompareLess: public IComp<std::string> {
public:
    StringCompareLess() {}

    bool operator()(const std::string & first, const std::string & second) const override {
        return first.length() < second.length();
    }
};

class StringCompareGreater: public IComp<std::string> {
public:
    StringCompareGreater() {}

    bool operator()(const std::string & first, const std::string & second) const override {
        return first.length() > second.length();
    }
};

#endif /* Comparators_h */


