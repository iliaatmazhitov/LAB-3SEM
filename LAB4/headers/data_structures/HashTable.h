//
// Created by Илья Атмажитов on 21.11.2024.
//

#ifndef LABORATORY4_HASHTABLE_H
#define LABORATORY4_HASHTABLE_H

#include <functional>
#include "../sequences/ArraySequence.h"


template <class Key, class Value, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
class HashTable {
private:
    ArraySequence<ArraySequence<std::pair<Key, Value>>> _table;
    size_t _count;
public:
    HashTable(): _table(10), _count(0) {}

    bool Find(const Key& key) {
        size_t index = Hash()(key) % _table.GetLength();

        for (size_t i = 0; i < _table.Get(index).GetLength(); i++) {
            if (Equal()(key, _table.Get(index).Get(i).first)) {
                return true;
            }
        }
        return false;
    }

    std::pair<Key, Value>* FindElem(const Key& key) {
        size_t index = Hash()(key) % _table.GetLength();

        for (size_t i = 0; i < _table.Get(index).GetLength(); i++) {
            if (Equal()(key, _table.Get(index).Get(i).first)) {
                return &(_table.Get(index).Get(i));
            }
        }
        return nullptr;
    }

    const std::pair<Key, Value>* FindElem(const Key& key) const{
        size_t index = Hash()(key) % _table.GetLength();

        for (size_t i = 0; i < _table.Get(index).GetLength(); i++) {
            if (Equal()(key, _table.Get(index).Get(i).first)) {
                return &(_table.Get(index).Get(i));
            }
        }
        return nullptr;
    }

    void Erase(const Key& key) {
        size_t index = Hash()(key) % _table.GetLength();

        for (size_t i = 0; i < _table.Get(index).GetLength(); i++) {
            if (Equal()(key, _table.Get(index).Get(i).first)) {
                size_t place = _table.Get(index).GetLength();
                std::swap(_table.Get(index).Get(i), _table.Get(index).Get(place - 1));
                _table.Get(index).PopBack();
                _count--;
                return;
            }
        }
    }

    ArraySequence<Key> GetKeys() const {
        ArraySequence<Key> keys;
        for (size_t i = 0; i < _table.GetLength(); i++) {
            for (size_t j = 0; j < _table.Get(i).GetLength(); j++) {
                keys.Append(_table.Get(i).Get(j).first);
            }
        }
        return keys;
    }

    void Insert(const Key& key, const Value & value) {
        if (Find(key)) {
            size_t index = Hash()(key) % _table.GetLength();
            for (size_t i = 0; i < _table.Get(index).GetLength(); i++) {
                if (Equal()(key, _table.Get(index).Get(i).first)) {
                    _table.Get(index).Get(i).second = value;
                    return;
                }
            }
            return;
        }

        if ((_count + 1) > _table.GetLength() * 0.8) {
            ArraySequence<ArraySequence<std::pair<Key, Value>>> new_table(_table.GetLength() * 2);

            for (size_t i = 0; i < _table.GetLength(); i++) {
                for (size_t j = 0 ; j < _table.Get(i).GetLength(); j++) {
                    size_t index = Hash()(_table.Get(i).Get(j).first) % new_table.GetLength();
                    new_table.Get(index).Append(_table.Get(i).Get(j));
                }
            }
            _table = new_table;
        }

        size_t index = Hash()(key) % _table.GetLength();
        _table.Get(index).Append({key, value});
        _count++;
    }

    template<class T, class U>
    friend std::ostream & operator<<(std::ostream & os, const HashTable<T, U> & table);
};

template <class Key, class Value>
std::ostream & operator<<(std::ostream & os, const HashTable<Key, Value> & table) {
    for (size_t i = 0; i < table._table.GetLength(); i++) {
        for (size_t j = 0; j < table._table.Get(i).GetLength(); j++) {
            auto & pair = table._table.Get(i).Get(j);
            os <<pair.first << ", " << pair.second << std::endl;
        }
    }
    return os;
}

#endif //LABORATORY4_HASHTABLE_H
