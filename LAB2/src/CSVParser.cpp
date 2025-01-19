//
// Created by Илья Атмажитов on 17.01.2025.
//


#include "CSVParser.h"
#include <fstream>
#include <regex>
#include <QDebug>

ArraySequence<Person>* CSVParser::readCSV(const std::string& filename, int limit) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    auto* sequence = new ArraySequence<Person>();
    std::string line;

    if (!std::getline(file, line)) {
        delete sequence;
        throw std::runtime_error("Empty file: " + filename);
    }

    int count = 0;
    while (std::getline(file, line) && (limit == -1 || count < limit)) {
        try {
            std::stringstream ss(line);
            Person person = {};
            std::string token;

            std::vector<std::string> fields;
            while (std::getline(ss, token, ',')) {
                fields.push_back(cleanField(token));
            }

            if (fields.size() < 16) {
                qDebug() << "Skipping line with insufficient fields:" << QString::fromStdString(line);
                continue;
            }

            try {
                person.fiscal_year = std::stoul(fields[0]);
            } catch (const std::exception& e) {
                qDebug() << "Invalid fiscal year:" << QString::fromStdString(fields[0]);
                continue;
            }

            person.agency_name = fields[1].substr(0, 255);
            person.last_name = fields[2].substr(0, 255);
            person.first_name = fields[3].substr(0, 255);
            person.mid_init = fields[4].substr(0, 10);
            person.agency_start_date = fields[5].substr(0, 20);
            person.work_location_borough = fields[6].substr(0, 255);
            person.title_description = fields[7].substr(0, 255);
            person.leave_status = fields[8].substr(0, 50);
            person.pay_basis = fields[10].substr(0, 50);

            try {
                person.base_salary = parseMonetaryValue(fields[9], person.pay_basis);
                person.regular_hours = parseHours(fields[11]);
                person.regular_gross_paid = parseMonetaryValue(fields[12], "");
                person.ot_hours = parseHours(fields[13]);
                person.total_ot_paid = parseMonetaryValue(fields[14], "");
                person.total_other_pay = parseMonetaryValue(fields[15], "");
            } catch (const std::exception& e) {
                qDebug() << "Error parsing numeric fields:" << e.what();
                continue;
            }

            sequence->Append(person);
            count++;

        } catch (const std::exception& e) {
            qDebug() << "Error parsing line:" << QString::fromStdString(line);
            qDebug() << "Error:" << e.what();
            continue;
        }
    }

    if (sequence->GetLength() == 0) {
        delete sequence;
        throw std::runtime_error("No valid data was read from file: " + filename);
    }

    return sequence;
}

std::string CSVParser::cleanField(std::string field) {
    if (field.empty()) return field;

    size_t start = field.find_first_not_of(" \t\r\n\"$");
    if (start == std::string::npos) return "";

    size_t end = field.find_last_not_of(" \t\r\n\"$");
    if (end == std::string::npos) return "";

    return field.substr(start, end - start + 1);
}

double CSVParser::parseMonetaryValue(const std::string& value, const std::string& payBasis) {
    if (value.empty() || value == "N/A" || value == "-" ||
        value == "ACTIVE" || value == "CEASED") {
        return 0.0;
    }

    try {
        std::string cleaned = cleanField(value);
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ','), cleaned.end());
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '$'), cleaned.end());

        if (cleaned.empty()) return 0.0;

        double amount = std::stod(cleaned);
        return convertToAnnualSalary(amount, payBasis);
    } catch (const std::exception& e) {
        return 0.0;
    }
}

size_t CSVParser::parseHours(const std::string& value) {
    if (value.empty() || value == "N/A" || value == "-" ||
        value.find("per") != std::string::npos) {
        return 0;
    }

    try {
        std::string cleaned = cleanField(value);
        if (cleaned.empty()) return 0;

        return static_cast<size_t>(std::stod(cleaned));
    } catch (const std::exception& e) {
        return 0;
    }
}

double CSVParser::convertToAnnualSalary(double amount, const std::string& payBasis) {
    if (amount < 0) return 0.0;

    if (payBasis.find("per Annum") != std::string::npos) {
        return amount;
    } else if (payBasis.find("per Day") != std::string::npos) {
        return amount * 260;
    } else if (payBasis.find("per Hour") != std::string::npos) {
        return amount * 2080; 
    }
    return amount;
}
