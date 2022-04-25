#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "company.h"

class Company;

class Employee{
    int employee_id;
    int employee_salary;
    int employee_grade;
    Company* company_ptr;

    public:
    Employee(int employee_id, int employee_salary, int employee_grade, Company* company_ptr);
    ~Employee() = default;
    
    void setCompany(Company* new_company_ptr){ company_ptr = new_company_ptr; }
    void setGrade(int new_grade){ employee_grade = new_grade; }
    void setSalary(int new_salary){ employee_salary = new_salary; } 
    int getId(){ return employee_id; }
    int getGrade(){ return employee_grade; }
    int getCompanyId();
    Company* getCompanyPtr(){ return company_ptr; }
    int getSalary(){ return employee_salary; }
    Employee operator=(Employee other_employee);
    bool operator<(Employee &other_employee);
    bool operator>(Employee &other_employee);
};

#endif  //EMPLOYEE_H