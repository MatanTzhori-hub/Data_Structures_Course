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

    ReturnValue addEmployee(int EmployeeID, int CompanyID, int Grade);
    ReturnValue removeEmployee(int EmployeeID);
    ReturnValue acquireCompany(int AcquirerID, int TargetID, double Factor);
    ReturnValue employeeSalaryIncrease(int EmployeeID, int SalaryIncrease);
    ReturnValue promoteEmployee(int EmployeeID, int BumpGrade);
    ReturnValue sumOfBumpGradeBetweenTopWorkersByCompany (int CompanyID, int m, long long* sumBumpGrade);
    ReturnValue averageBumpGradeBetweenSalaryByCompany (int CompanyID, int lowerSalary, int higherSalary, double* averageBumpGrade);
    ReturnValue companyValue(int CompanyID, double* standing);
    ReturnValue bumpGradeToEmployees(int lowerSalary, int higherSalary, int BumpGrade);

    int getSize(){ return size; }
};


/* todo :
    1.  When removing an employee from all employees tree, we calculate the real grade of another employee and set it to it.
        we need to make sure we fix the sumgrades in the tree of the specific company.
    
    2.  When bumping the grade of all employees in the data struct, we need to also do the same bump to each tree in each company 
        (on the same range)

    3.  Need to think about what to do with the grades when combaining 2 trees, because it affects the tree of all employees.

    4.  
*/


#endif // COMPANIES_MANAGER_H