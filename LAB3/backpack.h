//
// Created by Илья Атмажитов on 19.12.2024.
//

#ifndef LABORATORY3_BACKPACK_H
#define LABORATORY3_BACKPACK_H

#include <cstddef>
#include "sequences/Sequence.h"
#include "sequences/ArraySequence.h"

using namespace std;

struct Item {
    size_t price;
    size_t weight;
};

ostream& operator<<(ostream& os, Item item);

class Backpack {
private:
    ArraySequence<Item> bp;
    size_t capacity;
public:
    Backpack(const ArraySequence<Item>& bp, size_t capacity): bp(bp), capacity(capacity) {}

    ArraySequence<Item> dynamic_backpack() {
        ArraySequence<ArraySequence<size_t>> matrix(bp.GetLength() + 1);
        for (int i = 0; i < matrix.GetLength(); i++) {
            matrix.Get(i) = ArraySequence<size_t>(capacity + 1);
        }

        for (int j = 0; j < matrix.Get(0).GetLength(); j++) {
            matrix.Get(0).Get(j) = 0;
        }

        for (int i = 1; i < matrix.GetLength(); i++) {
            for (int j = 0; j <= capacity; ++j) {
                if (bp.Get(i - 1).weight <= j) {
                    matrix.Get(i).Get(j) = max(bp.Get(i-1).price + matrix.Get(i-1).Get(j - bp.Get(i-1).weight), matrix.Get(i-1).Get(j));
                } else {
                    matrix.Get(i).Get(j) = matrix.Get(i-1).Get(j);
                }

            }
        }

        size_t x = capacity;
        size_t y = bp.GetLength();
        ArraySequence<Item> result;

        while (x > 0 && y > 0) {
            if(matrix.Get(y-1).Get(x) == matrix.Get(y).Get(x)) {
                y--;
                continue;
            }
            result.Append(bp.Get(y-1));
            x -= bp.Get(y-1).weight;
            y--;
        }
        return result;
    }

};

#endif //LABORATORY3_BACKPACK_H
