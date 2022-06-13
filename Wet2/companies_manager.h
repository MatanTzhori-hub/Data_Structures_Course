#ifndef COMPANIES_MANAGER_H
#define COMPANIES_MANAGER_H

#include "employee.h"
#include "union.h"


class CompaniesManager{
    Union<Company*> companies_union;
    int size;
    
    public:
    CompaniesManager(int size);
    ~CompaniesManager();

    ReturnValue addEmployee(int EmployeeID, int CompanyID, long long Grade);
    ReturnValue removeEmployee(int EmployeeID);
    ReturnValue acquireCompany(int AcquirerID, int TargetID, double Factor);
    ReturnValue employeeSalaryIncrease(int EmployeeID, int SalaryIncrease);
    ReturnValue promoteEmployee(int EmployeeID, long long BumpGrade);
    ReturnValue sumOfBumpGradeBetweenTopWorkersByCompany (int CompanyID, int m, long long* sumBumpGrade);
    ReturnValue averageBumpGradeBetweenSalaryByCompany (int CompanyID, int lowerSalary, int higherSalary, double* averageBumpGrade);
    ReturnValue companyValue(int CompanyID, double* standing);
    ReturnValue bumpGradeToEmployees(int lowerSalary, int higherSalary, long long BumpGrade);

    int getSize(){ return size; }
};


#endif // COMPANIES_MANAGER_H