#ifndef COMPANIES_MANAGER_H
#define COMPANIES_MANAGER_H

#include "employee.h"
#include "rank_tree.h"

class CompaniesManager{
    RankTree<EmployeeKey, Employee*> RankTree;
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

};

template<typename key, typename data>
void deleteTreeData(Tree<key,data*> &tree){
    auto iter = tree.begin();
    while(iter != tree.end()){
        data* cur_data = iter.getData();
        delete cur_data;
        iter.next();
    }
}

#endif // COMPANIES_MANAGER_H