#include "companies_manager.h"
#include <exception>

CompaniesManager::CompaniesManager(): all_employees_salary_filtered(),
                                      all_employees_id_filtered(),
                                      all_companies(),
                                      non_empty_companies(),
                                      highest_earner(nullptr){}

CompaniesManager::~CompaniesManager(){
    deleteTreeData(all_employees_salary_filtered);
    deleteTreeData(all_companies);
}

ReturnValue CompaniesManager::AddCompany(int CompanyID, int Value){
    try{
        Company* new_company = new Company(CompanyID, Value);
        ReturnValue res = all_companies.insert(CompanyID, new_company);
        if(res != MY_SUCCESS){
            return res;
        }
    }
    catch(std::bad_alloc err){
        return MY_ALLOCATION_ERROR;
    }
    return MY_SUCCESS;
}

ReturnValue CompaniesManager::AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade){
    auto employee_iter = all_employees_id_filtered.findElement(EmployeeID);
    if(employee_iter != all_employees_id_filtered.end()){
        return MY_FAILURE;
    }

    auto company_iter = all_companies.findElement(CompanyID);
    if(company_iter == all_companies.end()){
        return MY_FAILURE;
    }

    Company *company_ptr = company_iter.getData();
    bool is_company_empty = (company_ptr->getSize() == 0);

    try{
        Employee* new_employee = new Employee(EmployeeID, Salary, Grade, company_ptr);
        Employee_Key new_employee_key = Employee_Key(EmployeeID, Salary);
        all_employees_id_filtered.insert(EmployeeID, new_employee);
        all_employees_salary_filtered.insert(new_employee_key, new_employee);
        company_ptr->addEmployee(*new_employee);
        
        if(is_company_empty){
            non_empty_companies.insert(CompanyID, company_ptr);
        }

        // Check if need to update highest_earner
        if(all_employees_salary_filtered.getSize() == 1){
            highest_earner = new_employee;
        }
        else if(*highest_earner < *new_employee){
            highest_earner = new_employee;
        }
    }
    catch(std::bad_alloc err){
        return MY_ALLOCATION_ERROR;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::RemoveEmployee(int EmployeeID){
    auto employee_iter = all_employees_id_filtered.findElement(EmployeeID);
    if(employee_iter == all_employees_id_filtered.end()){
        return MY_FAILURE;
    }
    else{
        Employee* employee_ptr = employee_iter.getData();
        Company* company_ptr = employee_ptr->getCompanyPtr();

        Employee_Key employee_key = Employee_Key(employee_ptr->getId(), employee_ptr->getSalary());
        all_employees_id_filtered.removeElement(EmployeeID);
        all_employees_salary_filtered.removeElement(employee_key);

        company_ptr->removeEmployee(*employee_ptr);

        // Check if need to update highest_earner
        if(highest_earner == employee_ptr){
            highest_earner = nullptr;
            if(all_employees_salary_filtered.getSize() != 0){
                Iterator<Employee_Key, Employee*> rightmost_iter = all_employees_salary_filtered.getRightMost();
                highest_earner = rightmost_iter.getData();
            }
        }

        if(company_ptr->getSize() == 0){
            non_empty_companies.removeElement(company_ptr->getId());
        }

        delete employee_ptr;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::RemoveCompany(int CompanyID){
    auto company_iter = all_companies.findElement(CompanyID);
    if(company_iter == all_companies.end()){
        return MY_FAILURE;
    }
    else{
        Company* company_ptr = company_iter.getData();
        if(company_ptr->getSize() > 0){
            return MY_FAILURE;
        }
        else{
            ReturnValue res = all_companies.removeElement(CompanyID);
            non_empty_companies.removeElement(CompanyID);
            if(res!=MY_SUCCESS)
                return res;
        }

        delete company_ptr;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees){
    auto company_iter = all_companies.findElement(CompanyID);
    if(company_iter == all_companies.end()){
        return MY_FAILURE;
    }

    Company* company_ptr = company_iter.getData();
    *Value = company_ptr->getValue();
    *NumEmployees = company_ptr->getSize();

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade){
    auto employee_iter = all_employees_id_filtered.findElement(EmployeeID);
    if(employee_iter == all_employees_id_filtered.end()){
        return MY_FAILURE;
    }

    Employee *employee_ptr = employee_iter.getData();
    *Salary = employee_ptr->getSalary();
    *Grade = employee_ptr->getGrade();
    
    Company *company_ptr = employee_ptr->getCompanyPtr();
    *EmployerID = company_ptr->getId();

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::IncreaseCompanyValue(int CompanyID, int ValueIncrease){
    auto company_iter = all_companies.findElement(CompanyID);
    if(company_iter == all_companies.end()){
        return MY_FAILURE;
    }

    Company* company_ptr = company_iter.getData();
    company_ptr->increaseValue(ValueIncrease);
    
    return MY_SUCCESS;
}

ReturnValue CompaniesManager::PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade){
    auto employee_iter = all_employees_id_filtered.findElement(EmployeeID);
    if(employee_iter == all_employees_id_filtered.end()){
        return MY_FAILURE;
    }

    Employee* employee_ptr = employee_iter.getData();
    Company* company_ptr = employee_ptr->getCompanyPtr();

    Employee_Key employee_key = Employee_Key(employee_ptr->getId(), employee_ptr->getSalary());
    all_employees_id_filtered.removeElement(EmployeeID);
    all_employees_salary_filtered.removeElement(employee_key);
    company_ptr->removeEmployee(*employee_ptr);

    employee_ptr->setSalary(employee_ptr->getSalary() + SalaryIncrease);
    if(BumpGrade > 0){
        employee_ptr->setGrade(employee_ptr->getGrade() + 1);
    }

    Employee_Key new_key = Employee_Key(employee_ptr->getId(), employee_ptr->getSalary());
    ReturnValue res1 = all_employees_id_filtered.insert(employee_ptr->getId(), employee_ptr);
    ReturnValue res2 = all_employees_salary_filtered.insert(new_key, employee_ptr);
    ReturnValue res3 = company_ptr->addEmployee(*employee_ptr);

    if(res1 != MY_SUCCESS || res2 != MY_SUCCESS || res3 != MY_SUCCESS){
        return MY_FAILURE;
    }

    // Check if need to update highest_earner
    if(*highest_earner < *employee_ptr){
        highest_earner = employee_ptr;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::HireEmployee(int EmployeeID, int NewCompanyID){
    auto employee_iter = all_employees_id_filtered.findElement(EmployeeID);
    if( employee_iter == all_employees_id_filtered.end()){
        return MY_FAILURE;
    }

    Employee *employee_ptr = employee_iter.getData();
    Company *curr_company_ptr = employee_ptr->getCompanyPtr();

    if (curr_company_ptr->getId() == NewCompanyID){
        return MY_FAILURE;
    }

    auto new_company_iter = all_companies.findElement(NewCompanyID);
    if(new_company_iter == all_companies.end()){
        return MY_FAILURE;
    }

    Company *new_company_ptr = new_company_iter.getData();

    curr_company_ptr->removeEmployee(*employee_ptr);
    if(curr_company_ptr->getSize() == 0){
        non_empty_companies.removeElement(curr_company_ptr->getId());
    }
    employee_ptr->setCompany(new_company_ptr);
    new_company_ptr->addEmployee(*employee_ptr);
    if(new_company_ptr->getSize() == 1){
        non_empty_companies.insert(new_company_ptr->getId(), new_company_ptr);
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::AcquireCompany(int AcquirerID, int TargetID, double Factor){
    auto acquire_company_iter = all_companies.findElement(AcquirerID);
    if(acquire_company_iter == all_companies.end()){
        return MY_FAILURE;
    }
    
    auto target_company_iter = all_companies.findElement(TargetID);
    if(target_company_iter == all_companies.end()){
        return MY_FAILURE;
    }
    
    Company* acquire_company_ptr = acquire_company_iter.getData();
    Company* target_company_ptr = target_company_iter.getData();
    
    if(acquire_company_ptr->getValue() < 10 * target_company_ptr->getValue()){
        return MY_FAILURE;
    }

    acquire_company_ptr->AcquireAnotherCompany(target_company_ptr, Factor);
    
    non_empty_companies.removeElement(TargetID);
    all_companies.removeElement(TargetID);

    acquire_company_ptr->updateCompanyForAllEmployees();

    delete target_company_ptr;
    target_company_ptr = nullptr;

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetHighestEarner(int CompanyID, int *EmployeeID){
    if(CompanyID < 0){
        if(highest_earner == nullptr){
            return MY_FAILURE;
        }
        else{
            *EmployeeID = highest_earner->getId();
            return MY_SUCCESS;
        }
    }
    else{
        auto company_iter = all_companies.findElement(CompanyID);
        if(company_iter == all_companies.end()){
            return MY_FAILURE;
        }

        Company* company_ptr = company_iter.getData();
        if(company_ptr->getHighestEarner() == nullptr){
            return MY_FAILURE;
        }
        else{
            *EmployeeID = company_ptr->getHighestEarner()->getId();
            return MY_SUCCESS;
        }
    }

    return MY_FAILURE;
}

ReturnValue CompaniesManager::GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees){
    if(CompanyID < 0){
        *NumOfEmployees = all_employees_salary_filtered.getSize();
        if(*NumOfEmployees <= 0){
            return MY_FAILURE;
        }
        else{
            int *Employees_arr = (int*)malloc(*NumOfEmployees * sizeof(int));
            if(Employees_arr == NULL){
                return MY_ALLOCATION_ERROR;
            }
            auto employee_iter = all_employees_salary_filtered.begin(-1);

            for(int i = 0; i < *NumOfEmployees; i++){
                Employees_arr[i] = employee_iter.getData()->getId();
                employee_iter.next();
            }

            *Employees = Employees_arr;
        }
    }
    else{
        auto company_iter = all_companies.findElement(CompanyID);
        if(company_iter == all_companies.end()){
            return MY_FAILURE;
        }

        Company* company_ptr = company_iter.getData();
        
        *NumOfEmployees = company_ptr->getSize();
        if(*NumOfEmployees <= 0){
            return MY_FAILURE;
        }
        else{
            return company_ptr->GetAllEmployeesBySalary(Employees, NumOfEmployees);
        }
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees){
    if(non_empty_companies.getSize() < NumOfCompanies){
        return MY_FAILURE;
    }
    int *Employees_arr = (int*)malloc(NumOfCompanies * sizeof(int));
    if(Employees_arr == NULL){
        return MY_ALLOCATION_ERROR;
    }

    auto company_iter = non_empty_companies.begin();

    for(int i = 0; i < NumOfCompanies; i++){
        Employees_arr[i] = company_iter.getData()->getHighestEarner()->getId();
        company_iter.next();
    }

    *Employees = Employees_arr;
    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary, 
                                                        int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees){
    
    if(CompanyID > 0){
        auto company_iter = all_companies.findElement(CompanyID);
        if(company_iter == all_companies.end() || company_iter.getData()->getSize() == 0){
            return MY_FAILURE;
        }

        Company* company_ptr = company_iter.getData();
        company_ptr->GetNumEmployeesMatching(MinEmployeeID, MaxEmployeeId, MinSalary, MinGrade, TotalNumOfEmployees, NumOfEmployees);
        return MY_SUCCESS;
    }
    else{
        if(all_employees_id_filtered.getSize() == 0){
            return MY_FAILURE;
        }

        int numEmployees = 0;
        int totalNumEmployees = 0;
        auto start_iter = all_employees_id_filtered.findCloseestElement(MinEmployeeID);
        auto end_iter = all_employees_id_filtered.findCloseestElement(MaxEmployeeId);

        while(start_iter != end_iter){
            Employee* employee_ptr = start_iter.getData();
            if( MinEmployeeID <= employee_ptr->getId() && employee_ptr->getId() <= MaxEmployeeId){
                totalNumEmployees++;
                if(MinSalary <= employee_ptr->getSalary() && MinGrade <= employee_ptr->getGrade()){
                    numEmployees++;
                }
            }
            start_iter.next();
        }

        // one more time, incase end_iter is the largest key, and is still in range.
        Employee* employee_ptr = start_iter.getData();
        if( MinEmployeeID <= employee_ptr->getId() && employee_ptr->getId() <= MaxEmployeeId){
            totalNumEmployees++;
            if(MinSalary <= employee_ptr->getSalary() && MinGrade <= employee_ptr->getGrade()){
                numEmployees++;
            }
        }

        *TotalNumOfEmployees = totalNumEmployees;
        *NumOfEmployees = numEmployees;
        return MY_SUCCESS;
    }

 }
