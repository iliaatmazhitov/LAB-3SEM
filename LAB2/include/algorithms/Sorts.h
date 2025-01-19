//
//  Sorts.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef Sorts_h
#define Sorts_h

#include "../sequences/Sequence.h"
#include "../sequences/ArraySequence.h"
#include "Comparators.h"

template <class T>
class ISort {
public:
    virtual void Sort(Sequence<T> & seq, IComp<T>& Comp) = 0;
};

template <class T>
class MergeSort: public ISort<T> {
private:
    void Merge(Sequence<T> & seq, Sequence<T> & buffer, size_t begin, size_t mid, size_t end, IComp<T>& comp) {
        size_t i = 0;
        size_t j = 0;

        while((begin + i) != mid && (mid + j) != end) {
            if (comp(seq.Get(mid + j), seq.Get(begin + i))) {
                buffer.Set(i + j, seq.Get(mid + j));
                ++j;
            } else {
                buffer.Set(i + j, seq.Get(begin + i));
                ++i;
            }
        }

        while((begin + i) != mid) {
            buffer.Set(i + j, seq.Get(begin + i));
            ++i;
        }

        while((mid + j) != end) {
            buffer.Set(i + j, seq.Get(mid + j));
            ++j;
        }

        for (size_t q = 0; q < i + j; q++) {
            seq.Set(begin + q, buffer.Get(q));
        }
    }

    void MergeSortRec(Sequence<T> & seq, Sequence<T> & buffer, size_t begin, size_t end, IComp<T>& comp) {
        if (end - begin < 2) {
            return;
        }
        size_t mid = (begin + end) / 2;
        MergeSortRec(seq, buffer, begin, mid, comp);
        MergeSortRec(seq, buffer, mid, end, comp);
        Merge(seq, buffer, begin, mid, end, comp);
    }

public:
    void Sort(Sequence<T> & seq, IComp<T> & comp) override {
        ArraySequence<T> buffer(seq.GetLength());
        MergeSortRec(seq, buffer, 0, seq.GetLength(), comp);
    }
};

template <class T>
class HeapSort: public ISort<T> {
private:
    void SiftDown(Sequence<T> & seq, size_t size, size_t index, IComp<T>& comp) {
        size_t index_left = 2 * index + 1;
        size_t index_right = 2 * index + 2;

        if (index_left >= size) {
            return;
        }

        size_t index_replace = index;
        if (comp(seq.Get(index_replace), seq.Get(index_left))) {
            index_replace = index_left;
        }

        if (index_right < size && comp(seq.Get(index_replace), seq.Get(index_right))) {
            index_replace = index_right;
        }

        if (index_replace == index) {
            return;
        }

        std::swap(seq.Get(index), seq.Get(index_replace));
        SiftDown(seq, size, index_replace, comp);
    }

public:
    void Sort(Sequence<T> & seq, IComp<T>& comp) override {
        for (size_t i = seq.GetLength(); i > 0; i--) {
            SiftDown(seq, seq.GetLength(), i - 1, comp);
        }

        for (size_t i = seq.GetLength(); i > 0; i--) {
            std::swap(seq.Get(0), seq.Get(i - 1));
            SiftDown(seq, i - 1, 0, comp);
        }
    }
};

#endif /* Sorts_h */