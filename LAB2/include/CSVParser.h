//
// Created by Илья Атмажитов on 17.01.2025.
//

#ifndef LABORATORY2_CSVPARSER_H
#define LABORATORY2_CSVPARSER_H

#include <string>
#include <sstream>
#include "sequences/ArraySequence.h"
#include "Person.h"

class CSVParser {
public:
    static ArraySequence<Person>* readCSV(const std::string& filename, int limit = -1);

private:
    static void cleanString(std::string& str);
    static double parseMonetaryValue(const std::string& value, const std::string& payBasis);
    static std::string cleanField(std::string field);
    static size_t parseHours(const std::string& value);
    static double convertToAnnualSalary(double amount, const std::string& payBasis);
};


#endif //LABORATORY2_CSVPARSER_H
