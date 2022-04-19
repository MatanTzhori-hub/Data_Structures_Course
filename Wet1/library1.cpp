#include "library1.h"
#include "companies_manager.h"

#define MIN_FACTOR 1.00

static StatusType retValToStatType(ReturnValue result){
    switch (result) {
        case MY_ALLOCATION_ERROR:
            return ALLOCATION_ERROR;
        case MY_FAILURE:
            return FAILURE;
        case MY_SUCCESS:
            return SUCCESS;
        case MY_INVALID_INPUT:
            return INVALID_INPUT;
        default:
            return FAILURE;
    }
}

void* Init(){
    try {
        CompaniesManager* companies_manager = new CompaniesManager();
        return companies_manager;
    }
    catch(std::bad_alloc err) {
        return nullptr;
    }
}

StatusType AddCompany(void *DS, int CompanyID, int Value){
    if (DS == nullptr || CompanyID <= 0 || Value <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->AddCompany(CompanyID, Value);
    return retValToStatType(res);
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade){
    if (DS == nullptr || EmployeeID <= 0 || CompanyID <= 0 || Salary <= 0 || Grade < 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->AddEmployee(EmployeeID, CompanyID, Salary, Grade);
    return retValToStatType(res);
}

StatusType RemoveCompany(void *DS, int CompanyID){
    if (DS == nullptr || CompanyID <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->RemoveCompany(CompanyID);
    return retValToStatType(res);
}

StatusType RemoveEmployee(void *DS, int EmployeeID){
    if (DS == nullptr || EmployeeID <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->RemoveEmployee(EmployeeID);
    return retValToStatType(res);
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees){
    if (DS == nullptr || CompanyID <= 0 || Value == nullptr || NumEmployees == nullptr){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetCompanyInfo(CompanyID, Value, NumEmployees);
    return retValToStatType(res);
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade){
    if (DS == nullptr || EmployeeID <= 0 || EmployerID == nullptr || Salary == nullptr || Grade == nullptr){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetEmployeeInfo(EmployeeID, EmployerID, Salary, Grade);
    return retValToStatType(res);
}

StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease){
    if (DS == nullptr || CompanyID <= 0 || ValueIncrease <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->IncreaseCompanyValue(CompanyID, ValueIncrease);
    return retValToStatType(res);
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade){
    if (DS == nullptr || EmployeeID <= 0 || SalaryIncrease <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->PromoteEmployee(EmployeeID, SalaryIncrease, BumpGrade);
    return retValToStatType(res);
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID){
    if (DS == nullptr || EmployeeID <= 0 || NewCompanyID <= 0){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->HireEmployee(EmployeeID, NewCompanyID);
    return retValToStatType(res);
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor){
    if (DS == nullptr || AcquirerID <= 0 || TargetID <= 0 || TargetID == AcquirerID || Factor < MIN_FACTOR){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->AcquireCompany(AcquirerID, TargetID, Factor);
    return retValToStatType(res);
}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID){
    if (DS == nullptr || CompanyID == 0 || EmployeeID == nullptr){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetHighestEarner(CompanyID, EmployeeID);
    return retValToStatType(res);
}

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees){
    if (DS == nullptr || CompanyID == 0 || Employees == nullptr || NumOfEmployees == nullptr){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetAllEmployeesBySalary(CompanyID, Employees, NumOfEmployees);
    return retValToStatType(res);
}

StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees){
    if (DS == nullptr || Employees == nullptr || NumOfCompanies < 1){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetHighestEarnerInEachCompany(NumOfCompanies, Employees);
    return retValToStatType(res);
}

StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId, 
                                    int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees){
    if (DS == nullptr || TotalNumOfEmployees == nullptr || NumOfEmployees == nullptr || CompanyID == 1 ||
        MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || MinEmployeeID > MaxEmployeeId){
        return INVALID_INPUT;
    }

    CompaniesManager* companies_manager = (CompaniesManager*)DS;
    ReturnValue res = companies_manager->GetNumEmployeesMatching(CompanyID, MinEmployeeID, MaxEmployeeId, 
                                                                    MinSalary, MinGrade, TotalNumOfEmployees, 
                                                                    NumOfEmployees);
    return retValToStatType(res);
}

void Quit(void** DS){
    if(!*DS){
        return;
    }

    delete DS;
    *DS = nullptr;
}


