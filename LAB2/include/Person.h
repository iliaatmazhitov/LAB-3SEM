//
//  Person.h
//  laboratory2
//
//  Created by Илья Атмажитов on 19.10.2024.
//

#ifndef Person_h
#define Person_h

#include <string>
#include <stdexcept>

using namespace std;

class Person {
public:
    size_t fiscal_year;
    string agency_name;
    string last_name;
    string first_name;
    string mid_init;
    string agency_start_date;
    string work_location_borough;
    string title_description;
    string leave_status;
    size_t base_salary;
    string pay_basis;
    size_t regular_hours;
    size_t regular_gross_paid;
    size_t ot_hours;
    size_t total_ot_paid;
    size_t total_other_pay;

    double getAnnualSalary() const {
        return base_salary;
    }

    Person(size_t fiscal_year = 0, string agency_name = "undefined", string last_name = "undefined", string first_name = "undefined",
           string mid_init = "undefined", string agency_start_date = "undefined", string work_location_borough = "undefined",
           string title_description = "undefined", string leave_status = "undefined", size_t base_salary = 0, string pay_basis = "undefined",
           size_t regular_hours = 0, size_t regular_gross_paid = 0, size_t ot_hours = 0, size_t total_ot_paid = 0, size_t total_other_pay = 0)
            : fiscal_year(fiscal_year), agency_name(agency_name), last_name(last_name), first_name(first_name), mid_init(mid_init),
              agency_start_date(agency_start_date), work_location_borough(work_location_borough), title_description(title_description),
              leave_status(leave_status), base_salary(base_salary), pay_basis(pay_basis), regular_hours(regular_hours),
              regular_gross_paid(regular_gross_paid), ot_hours(ot_hours), total_ot_paid(total_ot_paid), total_other_pay(total_other_pay) {}

    bool operator<(const Person& other) const {
        if (last_name != other.last_name)
            return last_name < other.last_name;

        if (first_name != other.first_name)
            return first_name < other.first_name;

        if (mid_init != other.mid_init)
            return mid_init < other.mid_init;

        if (agency_name != other.agency_name)
            return agency_name < other.agency_name;

        if (fiscal_year != other.fiscal_year)
            return fiscal_year < other.fiscal_year;

        if (agency_start_date != other.agency_start_date)
            return agency_start_date < other.agency_start_date;

        if (work_location_borough != other.work_location_borough)
            return work_location_borough < other.work_location_borough;

        if (title_description != other.title_description)
            return title_description < other.title_description;

        if (leave_status != other.leave_status)
            return leave_status < other.leave_status;

        if (base_salary != other.base_salary)
            return base_salary < other.base_salary;

        if (pay_basis != other.pay_basis)
            return pay_basis < other.pay_basis;

        if (regular_hours != other.regular_hours)
            return regular_hours < other.regular_hours;

        if (regular_gross_paid != other.regular_gross_paid)
            return regular_gross_paid < other.regular_gross_paid;

        if (ot_hours != other.ot_hours)
            return ot_hours < other.ot_hours;

        if (total_ot_paid != other.total_ot_paid)
            return total_ot_paid < other.total_ot_paid;

        return total_other_pay < other.total_other_pay;
    }

    bool operator>(const Person& other) const {
        if (last_name != other.last_name)
            return last_name > other.last_name;

        if (first_name != other.first_name)
            return first_name > other.first_name;

        if (mid_init != other.mid_init)
            return mid_init > other.mid_init;

        if (agency_name != other.agency_name)
            return agency_name > other.agency_name;

        if (fiscal_year != other.fiscal_year)
            return fiscal_year > other.fiscal_year;

        if (agency_start_date != other.agency_start_date)
            return agency_start_date > other.agency_start_date;

        if (work_location_borough != other.work_location_borough)
            return work_location_borough > other.work_location_borough;

        if (title_description != other.title_description)
            return title_description > other.title_description;

        if (leave_status != other.leave_status)
            return leave_status > other.leave_status;

        if (base_salary != other.base_salary)
            return base_salary > other.base_salary;

        if (pay_basis != other.pay_basis)
            return pay_basis > other.pay_basis;

        if (regular_hours != other.regular_hours)
            return regular_hours > other.regular_hours;

        if (regular_gross_paid != other.regular_gross_paid)
            return regular_gross_paid > other.regular_gross_paid;

        if (ot_hours != other.ot_hours)
            return ot_hours > other.ot_hours;

        if (total_ot_paid != other.total_ot_paid)
            return total_ot_paid > other.total_ot_paid;

        return total_other_pay > other.total_other_pay;
    }

    friend istream& operator>>(istream& is, Person& person);
    friend ostream& operator<<(ostream& os, const Person& person);
};

// Add inline keyword to stream operator implementations
inline istream& operator>>(istream& is, Person& person) {
    is >> person.fiscal_year >> person.agency_name >> person.last_name >> person.first_name >> person.mid_init
       >> person.agency_start_date >> person.work_location_borough >> person.title_description >> person.leave_status
       >> person.base_salary >> person.pay_basis >> person.regular_hours >> person.regular_gross_paid >> person.ot_hours
       >> person.total_ot_paid >> person.total_other_pay;
    return is;
}

inline ostream& operator<<(ostream& os, const Person& person) {
    os << person.fiscal_year << "," << person.agency_name << "," << person.last_name << "," << person.first_name << ","
       << person.mid_init << "," << person.agency_start_date << "," << person.work_location_borough << "," << person.title_description << ","
       << person.leave_status << "," << person.base_salary << "," << person.pay_basis << "," << person.regular_hours << ","
       << person.regular_gross_paid << "," << person.ot_hours << "," << person.total_ot_paid << "," << person.total_other_pay;
    return os;
}

#endif /* Person_h */

