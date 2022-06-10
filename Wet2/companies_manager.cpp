#include "companies_manager.h"
#include <exception>


CompaniesManager::CompaniesManager(int size): companies_union(size+1), size(size){
    for (int i=0; i <= size; i++){
        Company* curr_company = new Company(i, i);
        companies_union.makeSet(i, curr_company);
    }
}

CompaniesManager::~CompaniesManager(){
    Company* company_0;
    companies_union.findDataPtrByIndex(0, &company_0);
    company_0->killAllEmployees();
    companies_union.killAllCompanies();
}

ReturnValue CompaniesManager::addEmployee(int EmployeeID, int CompanyID, int Grade){
    Company* company_0;
    companies_union.findDataPtrByIndex(0, &company_0);
    ReturnValue res = MY_FAILURE;
    Employee* employee_ptr = company_0->findEmployee(EmployeeID, &res);

    if(res != ELEMENT_DOES_NOT_EXIST){
        return res;
    }
    
    Company* real_company;
    companies_union.findDataPtrByIndex(CompanyID, &real_company);
    try{
        Employee* new_employee = new Employee(EmployeeID, 0, Grade, real_company, company_0);
        real_company->addEmployee(*new_employee);
        company_0->addEmployee(*new_employee);
        }
    catch(std::bad_alloc &err){
        return MY_ALLOCATION_ERROR;
    }
    return MY_SUCCESS;
}


ReturnValue CompaniesManager::removeEmployee(int EmployeeID){
    Company* company_0;
    companies_union.findDataPtrByIndex(0, &company_0);
    ReturnValue res = MY_FAILURE;
    Employee* employee_ptr = company_0->findEmployee(EmployeeID, &res);

    if(res != ELEMENT_EXISTS){
        return res;
    }

    Company* real_company = employee_ptr->getCompanyPtr();
    real_company->removeEmployee(EmployeeID);
    company_0->removeEmployee(EmployeeID);
    return MY_SUCCESS;
}


// todo : think with adi how to implement the acquireAnotherCompany function
//        withou hurting the grade in the all employees tree.
ReturnValue CompaniesManager::acquireCompany(int AcquirerID, int TargetID, double Factor){

    return companies_union.unify(AcquirerID, TargetID, Factor);
}


ReturnValue CompaniesManager::employeeSalaryIncrease(int EmployeeID, int SalaryIncrease){
    Company* company_0;
    companies_union.findDataPtrByIndex(0, &company_0);
    ReturnValue res = MY_FAILURE;
    Employee* employee_ptr = company_0->findEmployee(EmployeeID, &res);

    if(res != ELEMENT_EXISTS){
        return res;
    }

    Company* real_company = employee_ptr->getCompanyPtr();
    company_0->employeeSalaryIncrease(EmployeeID, SalaryIncrease);
    real_company->employeeSalaryIncrease(EmployeeID, 0);

    return MY_SUCCESS;
}


ReturnValue CompaniesManager::promoteEmployee(int EmployeeID, int BumpGrade){
    Company* company_0;
    companies_union.findDataPtrByIndex(0, &company_0);
    ReturnValue res = MY_FAILURE;
    Employee* employee_ptr = company_0->findEmployee(EmployeeID, &res);

    if(res != ELEMENT_EXISTS){
        return res;
    }

    Company* real_company = employee_ptr->getCompanyPtr();
    company_0->promoteEmployee(EmployeeID, BumpGrade);
    real_company->promoteEmployee(EmployeeID, 0);

    return MY_SUCCESS;
}


ReturnValue CompaniesManager::sumOfBumpGradeBetweenTopWorkersByCompany (int CompanyID, int m, long long * sumBumpGrade){
    
    Company* company;
    ReturnValue res = companies_union.findDataPtrByIndex(CompanyID, &company);

    if(res != MY_SUCCESS || company->getPayedEmployeesNum() < m){
        return MY_FAILURE;
    }

    *sumBumpGrade = company->calcSumGradeOfmTop(m);

    return MY_SUCCESS;
}


ReturnValue CompaniesManager::averageBumpGradeBetweenSalaryByCompany (int CompanyID, int lowerSalary, int higherSalary, double* averageBumpGrade){
    double avg;
    Company* company;
    companies_union.findDataPtrByIndex(CompanyID, &company);

    ReturnValue res = company->calcAvgGradeInRange(lowerSalary, higherSalary, &avg);

    if(res != MY_SUCCESS){
        return res;
    }
    else{
        *averageBumpGrade = avg;
        return MY_SUCCESS;
    }
}


ReturnValue CompaniesManager::companyValue(int CompanyID, double* standing){
    double value = companies_union.calcTrueValue(CompanyID);
    *standing = value;

    return MY_SUCCESS;
}


ReturnValue CompaniesManager::bumpGradeToEmployees(int lowerSalary, int higherSalary, int BumpGrade){
    ReturnValue res = companies_union.bumpGradeToEmployees(lowerSalary, higherSalary, BumpGrade);
    return res;
}

