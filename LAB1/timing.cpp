//
//  timing.cpp
//  laboratory1
//
//  Created by Илья Атмажитов on 11.10.2024.
//

#include "../include/timing.h"
#include <chrono>
#include <utility>
#include <string>
#include "../include/gnuplot-iostream.h"




TimingResults runTimingTests() {
    std::vector<int> sizes = {1000, 10000, 100000, 1000000};
    TimingResults results;

    results.smartDynamic = timeAddition<int>(true, true, sizes);
    results.smartLinked = timeAddition<int>(true, false, sizes);
    results.weakDynamic = timeAddition<int>(false, true, sizes);
    results.weakLinked = timeAddition<int>(false, false, sizes);

    return results;
}



void plotTimingResults(const TimingResults& results) {
    Gnuplot gp;

    gp << "set title 'Время добавления указателей'\n";
    gp << "set xlabel 'Количество указателей'\n";
    gp << "set ylabel 'Время (секунды)'\n";
    gp << "set key outside\n";
    gp << "set logscale x\n";
    gp << "set logscale y\n";

    std::vector<std::pair<int, double>> smartDynamic = results.smartDynamic;
    std::vector<std::pair<int, double>> smartLinked = results.smartLinked;
    std::vector<std::pair<int, double>> weakDynamic = results.weakDynamic;
    std::vector<std::pair<int, double>> weakLinked = results.weakLinked;

    gp << "plot '-' with linespoints title 'Smart Dynamic', "
          "'-' with linespoints title 'Smart Linked', "
          "'-' with linespoints title 'Weak Dynamic', "
          "'-' with linespoints title 'Weak Linked'\n";

    gp.send1d(smartDynamic);
    gp.send1d(smartLinked);
    gp.send1d(weakDynamic);
    gp.send1d(weakLinked);

    std::cout << "Нажмите Enter для продолжения...";
    std::cin.get();
}
