#ifndef EMPLOYEE_RANK_H
#define EMPLOYEE_RANK_H

#include "employee.h"

class Employee;

class EmployeeRank{
    friend class RankTree<key_t, data_t, rank_t>;

    int weight;
    long sum_of_grades;
    int grade_bump;

public:
    explicit EmployeeRank(int grade);
    ~EmployeeRank() = default;

    void initializeRank(Employee employee);
    int getWeight() { return weight; }
    long getSumOfGrades() { return sum_of_grades; }
    int getGradeBump() { return grade_bump;}
    long setSumOfGrades(int new_sum) { sum_of_grades = new_sum; }
    int setGradeBump(int new_bump) { grade_bump = new_bump;}
    EmployeeRank& operator+=(EmployeeRank& other_employee_rank);
    EmployeeRank& operator-=(EmployeeRank& other_employee_rank);
};

#endif //EMPLOYEE_RANK_H