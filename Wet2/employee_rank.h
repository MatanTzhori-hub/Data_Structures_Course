#ifndef EMPLOYEE_RANK_H
#define EMPLOYEE_RANK_H

#include "employee.h"

class Employee;

class EmployeeRank{
    friend class RankTree<EmployeeKey, Employee*, EmployeeRank>;

    int weight;
    long long sum_of_grades;
    long long grade_bump;

public:
    explicit EmployeeRank(int grade);
    explicit EmployeeRank() = default;
    ~EmployeeRank() = default;

    void initializeRank(Employee employee);
    int getWeight() { return weight; }
    long long getSumOfGrades() { return sum_of_grades; }
    long long getGradeBump() { return grade_bump;}
    void setSumOfGrades(int new_sum) { sum_of_grades = new_sum; }
    void setGradeBump(int new_bump) { grade_bump = new_bump;}
    EmployeeRank& operator+=(EmployeeRank& other_employee_rank);
    EmployeeRank& operator-=(EmployeeRank& other_employee_rank);
};

#endif //EMPLOYEE_RANK_H