//
//  timing.h
//  laboratory1
//
//  Created by Илья Атмажитов on 11.10.2024.
//

#ifndef timing_h
#define timing_h


#include <chrono>
#include <vector>
#include <utility>
#include <string>
#include "gnuplot-iostream.h"
#include "SmartPointer.h"
#include "LinkedList.h"
#include "DynamicArray.h"


template<typename T>
std::vector<std::pair<int, double>> timeAddition(bool isSmartPtr, bool isDynamicArray, const std::vector<int>& sizes) {
    std::vector<std::pair<int, double>> results;

    for (int size : sizes) {
        auto start = std::chrono::high_resolution_clock::now();

        if (isDynamicArray) {
            DynamicArray<SmrtPtr<T>> smartArray;
            DynamicArray<WeakPtr<T>> weakArray;

            for (int i = 0; i < size; ++i) {
                if (isSmartPtr) {
                    smartArray.Append(SmrtPtr<T>(new T(i)));
                } else {
                    weakArray.Append(WeakPtr<T>());
                }
            }
        } else {
            LinkedList<SmrtPtr<T>> smartList;
            LinkedList<WeakPtr<T>> weakList;

            for (int i = 0; i < size; ++i) {
                if (isSmartPtr) {
                    smartList.Append(SmrtPtr<T>(new T(i)));
                } else {
                    weakList.Append(WeakPtr<T>());
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        results.push_back({size, diff.count()});
    }

    return results;
}


struct TimingResults {
    std::vector<std::pair<int, double>> smartDynamic;
    std::vector<std::pair<int, double>> smartLinked;
    std::vector<std::pair<int, double>> weakDynamic;
    std::vector<std::pair<int, double>> weakLinked;
};

TimingResults runTimingTests();

void plotTimingResults(const TimingResults& results);




#endif /* timing_h */
