#include "library2.h"
#include "companies_manager.h"

void *Init(int k){
    CompaniesManager* manager = new CompaniesManager(k);
    return (void*)manager;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || employeeID <= 0 || companyID <= 0 || companyID > manager->getSize() || grade < 0){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->addEmployee(employeeID, companyID, grade);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int employeeID){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || employeeID <= 0){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->removeEmployee(employeeID);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || factor <= 0 || companyID1 <= 0 || companyID1 > manager->getSize() ||
        companyID1 == companyID2 || companyID2 <= 0 || companyID2 > manager->getSize()){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->acquireCompany(companyID1, companyID2, factor);

    if(res != MY_SUCCESS){
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || employeeID <= 0 || salaryIncrease <= 0){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->employeeSalaryIncrease(employeeID, salaryIncrease);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || employeeID <= 0){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->promoteEmployee(employeeID, bumpGrade);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || companyID < 0 || companyID > manager->getSize() || m <= 0){
        return INVALID_INPUT;
    }

    long long sum_grade = 0;
    ReturnValue res = manager->sumOfBumpGradeBetweenTopWorkersByCompany(companyID, m, &sum_grade);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }
    else{
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", sum_grade);
        return SUCCESS;
    }
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || lowerSalary < 0 || higherSalary < 0 || higherSalary < lowerSalary || companyID < 0 || companyID > manager->getSize()){
        return INVALID_INPUT;
    }

    double avg = 0;
    ReturnValue res = manager->averageBumpGradeBetweenSalaryByCompany(companyID, lowerSalary, higherSalary, &avg);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    else if(res != MY_SUCCESS){
        return FAILURE;
    }
    else{
        printf("averageBumpGradeBetweenSalaryByCompany: %1f\n", avg);
        return SUCCESS;
    }
}

StatusType CompanyValue(void *DS, int companyID){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || companyID < 0 || companyID > manager->getSize()){
        return INVALID_INPUT;
    }

    double standing_value = 0;
    ReturnValue res = manager->companyValue(companyID, &standing_value);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }

    printf("CompanyValue: %1f\n", standing_value);
    return SUCCESS;
}

StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade){
    CompaniesManager* manager = (CompaniesManager*)DS;

    if(DS == NULL || higherSalary < lowerSalary || bumpGrade <= 0){
        return INVALID_INPUT;
    }

    ReturnValue res = manager->bumpGradeToEmployees(lowerSalary, higherSalary, bumpGrade);

    if(res == MY_ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

void Quit(void** DS){
    if(DS != NULL){
        CompaniesManager** manager = (CompaniesManager**)DS;
        delete *manager;
    }
}