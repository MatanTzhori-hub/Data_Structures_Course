#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "company.h"

class Company;

class Employee{
    int employee_id;
    int employee_salary;
    int employee_grade;
    int company_id;

    public:
    Employee(int employee_id, int employee_salary, int employee_grade, int company_id);
    ~Employee() = default;
    
    void setCompany(int new_company_id){ company_id = new_company_id; }
    void setGrade(int new_grade){ employee_grade = new_grade; }
    void setSalary(int new_salary){ employee_salary = new_salary; } 
    int getId(){ return employee_id; }
    int getGrade(){ return employee_grade; }
    int getCompanyId();
    int getSalary(){ return employee_salary; }
    Employee operator=(Employee other_employee);
    bool operator<(Employee &other_employee);
    bool operator>(Employee &other_employee);
};

#endif  //EMPLOYEE_H