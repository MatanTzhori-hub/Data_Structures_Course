#ifndef EMPLOYEE_RANK_H
#define EMPLOYEE_RANK_H

#include "employee.h"

class Employee;

class EmployeeRank{
    friend class RankTree<EmployeeKey, Employee*, EmployeeRank>;

    int weight;
    long long tree_grade;
    long long sum_of_grades;
    long long grade_bump;

public:
    explicit EmployeeRank(long long grade);
    explicit EmployeeRank() = default;
    ~EmployeeRank() = default;

    void initializeRank(Employee employee);
    int getWeight() { return weight; }
    long long getSumOfGrades() { return sum_of_grades; }
    long long getGradeBump() { return grade_bump;}
    long long getTreeGrade() { return tree_grade;}
    void setTreeGrade(long long new_grade) { tree_grade = new_grade; }
    void setSumOfGrades(long long new_sum) { sum_of_grades = new_sum; }
    void setGradeBump(long long new_bump) { grade_bump = new_bump;}
    EmployeeRank& operator+=(EmployeeRank& other_employee_rank);
    EmployeeRank& operator-=(EmployeeRank& other_employee_rank);
    EmployeeRank& operator=(EmployeeRank& other) = default;
};

#endif //EMPLOYEE_RANK_H