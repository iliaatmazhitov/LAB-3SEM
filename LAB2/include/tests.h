//
// Created by Илья Атмажитов on 27.12.2024.
//

#ifndef LABORATORY2_TESTS_H
#define LABORATORY2_TESTS_H

#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "../sequences/ArraySequence.h"
#include "../sorts/Sorts.h"
#include "../data_structures/Person.h"

class PersonSalaryComp : public IComp<Person> {
public:
    bool operator()(const Person& first, const Person& second) const override {
        return first.base_salary < second.base_salary;
    }
};

class PersonNameComp : public IComp<Person> {
public:
    bool operator()(const Person& first, const Person& second) const override {
        if (first.last_name != second.last_name)
            return first.last_name < second.last_name;
        return first.first_name < second.first_name;
    }
};

ArraySequence<Person>* readCSV(const string& filename, int limit = -1) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    ArraySequence<Person>* sequence = new ArraySequence<Person>();
    string line;
    getline(file, line);

    int count = 0;
    while (getline(file, line) && (limit == -1 || count < limit)) {
        stringstream ss(line);
        Person person;
        string token;

        getline(ss, token, ','); person.fiscal_year = stoul(token);
        getline(ss, person.agency_name, ',');
        getline(ss, person.last_name, ',');
        getline(ss, person.first_name, ',');
        getline(ss, person.mid_init, ',');
        getline(ss, person.agency_start_date, ',');
        getline(ss, person.work_location_borough, ',');
        getline(ss, person.title_description, ',');
        getline(ss, person.leave_status, ',');
        getline(ss, token, ','); person.base_salary = stoul(token);
        getline(ss, person.pay_basis, ',');
        getline(ss, token, ','); person.regular_hours = stoul(token);
        getline(ss, token, ','); person.regular_gross_paid = stoul(token);
        getline(ss, token, ','); person.ot_hours = stoul(token);
        getline(ss, token, ','); person.total_ot_paid = stoul(token);
        getline(ss, token, ','); person.total_other_pay = stoul(token);

        sequence->Append(person);
        count++;
    }

    return sequence;
}

template<class T>
bool isSorted(Sequence<T>& seq, IComp<T>& comp) {
    for (size_t i = 1; i < seq.GetLength(); i++) {
        if (comp(seq.Get(i), seq.Get(i-1))) {
            return false;
        }
    }
    return true;
}

class SortingTest : public ::testing::Test {
protected:
    ArraySequence<Person>* smallData;
    ArraySequence<Person>* mediumData;
    MergeSort<Person> mergeSort;
    HeapSort<Person> heapSort;
    Less<Person> lessComp;
    Greater<Person> greaterComp;
    PersonSalaryComp salaryComp;
    PersonNameComp nameComp;

    void SetUp() override {
        smallData = readCSV("data/people.csv", 100);
        mediumData = readCSV("data/people.csv", 1000);
    }

    void TearDown() override {
        delete smallData;
        delete mediumData;
    }
};

TEST_F(SortingTest, MergeSortWithLessComparator) {
mergeSort.Sort(*smallData, lessComp);
EXPECT_TRUE(isSorted(*smallData, lessComp));
}

TEST_F(SortingTest, MergeSortWithGreaterComparator) {
mergeSort.Sort(*smallData, greaterComp);
EXPECT_TRUE(isSorted(*smallData, greaterComp));
}

TEST_F(SortingTest, MergeSortWithSalaryComparator) {
mergeSort.Sort(*smallData, salaryComp);
EXPECT_TRUE(isSorted(*smallData, salaryComp));
}

TEST_F(SortingTest, MergeSortWithNameComparator) {
mergeSort.Sort(*smallData, nameComp);
EXPECT_TRUE(isSorted(*smallData, nameComp));
}

TEST_F(SortingTest, HeapSortWithLessComparator) {
heapSort.Sort(*smallData, lessComp);
EXPECT_TRUE(isSorted(*smallData, lessComp));
}

TEST_F(SortingTest, HeapSortWithGreaterComparator) {
heapSort.Sort(*smallData, greaterComp);
EXPECT_TRUE(isSorted(*smallData, greaterComp));
}

TEST_F(SortingTest, HeapSortWithSalaryComparator) {
heapSort.Sort(*smallData, salaryComp);
EXPECT_TRUE(isSorted(*smallData, salaryComp));
}

TEST_F(SortingTest, HeapSortWithNameComparator) {
heapSort.Sort(*smallData, nameComp);
EXPECT_TRUE(isSorted(*smallData, nameComp));
}

TEST_F(SortingTest, MergeSortPerformanceTest) {
mergeSort.Sort(*mediumData, lessComp);
EXPECT_TRUE(isSorted(*mediumData, lessComp));
}

TEST_F(SortingTest, HeapSortPerformanceTest) {
heapSort.Sort(*mediumData, lessComp);
EXPECT_TRUE(isSorted(*mediumData, lessComp));
}

#endif //LABORATORY2_TESTS_H