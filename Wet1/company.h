#ifndef COMPANY_H
#define COMPANY_H

#include "tree.h"
#include "employee_key.h"
#include "employee.h"

class Company{
    int company_id;
    int company_value;
    Employee* highest_earner;
    Tree<Employee_Key,Employee*> employees_salary_filtered;
    Tree<int,Employee*> employees_id_filtered;

    public:
    Company(int company_id, int company_value);
    ~Company();
    int getSize() const;
    int getId() const;
    int getValue() const;
    Employee* getHighestEarner() const;
    void increaseValue(int new_value);
    void setHighestEarner(Employee* new_highest_ptr);
    void setValue(int new_value);
    ReturnValue findEmployee(int employee_id, Employee* returned_employee);
    bool isEmployeeExist(int employee_id);
    ReturnValue addEmployee(Employee& employee);
    ReturnValue removeEmployee(Employee& employee);
    ReturnValue AcquireAnotherCompany(Company* other, double Factor);
};

#endif // COMPANY_H