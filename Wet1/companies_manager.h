#ifndef COMPANIES_MANAGER_H
#define COMPANIES_MANAGER_H

#include "employee.h"

class CompaniesManager{
    Tree<Employee_Key,Employee*> all_employees_salary_filtered;
    Tree<int,Employee*> all_employees_id_filtered;
    Tree<int,Company*> all_companies;
    Tree<int,Company*> non_empty_companies;
    Employee* highest_earner;

    public:
    CompaniesManager();
    ~CompaniesManager();

    ReturnValue AddCompany(int CompanyID, int Value);
    ReturnValue AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade);
    ReturnValue RemoveEmployee(int EmployeeID);
    ReturnValue RemoveCompany(int CompanyID);
    ReturnValue GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees);
    ReturnValue GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade);
    ReturnValue IncreaseCompanyValue(int CompanyID, int ValueIncrease);
    ReturnValue PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade);
    ReturnValue HireEmployee(int EmployeeID, int NewCompanyID);
    ReturnValue AcquireCompany(int AcquirerID, int TargetID, double Factor);
    ReturnValue GetHighestEarner(int CompanyID, int *EmployeeID);
    ReturnValue GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees);
    ReturnValue GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees);
    ReturnValue GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary, 
                                        int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees);

    void updateCompanyForAllEmployees(Company* new_company);
    void recursiveUpdateGroupForAllPlayers(Company* new_company, Iterator<Employee_Key, Employee*> iter);
};

template<typename key, typename data>
void deleteTreeData(Tree<key,data*> &tree){
    auto iter = tree.begin();
    recursiveDeleteTreeData(&iter);
}

template<typename key, typename data>
void recursiveDeleteTreeData(Tree<key,data*> &tree, Iterator<key, data*> &iter){
    if (iter.getPtr() == nullptr) {
            return;	
    }

    if (!iter.checkNullLeft()) {
        iter.goLeft();
        recursiveDeleteTreeData(iter);
        iter.goFather();
    }

    if (!iter.checkNullRight()) {
        iter.goRight();
        recursiveDeleteTreeData(iter);
        iter.goFather();
    }

    delete *(iter.getData());
}

#endif // COMPANIES_MANAGER_H