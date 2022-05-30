#ifndef EMPLOYEE_RANK_H
#define EMPLOYEE_RANK_H

#include "employee.h"

class Employee;

class EmployeeRank{
    int weight;
    long sum_of_grades;

public:
    explicit EmployeeRank(int grade);
    ~EmployeeRank() = default;

    void initializeRank(Employee employee);
    int getWeight() { return weight; }
    long getSumOfGrades() { return sum_of_grades; }
    EmployeeRank& operator+=(EmployeeRank& other_employee_rank);
    EmployeeRank& operator-=(EmployeeRank& other_employee_rank);
};

#endif //EMPLOYEE_RANK_H