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
        if(res == MY_FAILURE){
            return res;
        }
    }
    catch(std::bad_alloc err){
        return MY_ALLOCATION_ERROR;
    }
    return MY_SUCCESS;
}

ReturnValue CompaniesManager::AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade){
    auto employee_iter = all_employees_id_filtered.begin();
    ReturnValue res = all_employees_id_filtered.findElement(EmployeeID, employee_iter);
    if(res == ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    auto company_iter = all_companies.begin();
    res = all_companies.findElement(CompanyID, company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    bool is_company_empty = (all_companies.getSize() == 0);
    Company *company_ptr = company_iter.getData();

    try{
        Employee* new_employee = new Employee(EmployeeID, Salary, Grade, company_ptr);
        Employee_Key new_employee_key = Employee_Key(EmployeeID, Salary);
        all_employees_id_filtered.insert(EmployeeID, new_employee);
        all_employees_salary_filtered.insert(new_employee_key, new_employee);
        company_ptr->addEmployee(*new_employee);
        
        if(is_company_empty){
            non_empty_companies.insert(CompanyID, company_ptr);
        }

        if(highest_earner->getSalary() < new_employee->getSalary()){
            highest_earner = new_employee;
        }
        else if (highest_earner->getSalary() == new_employee->getSalary())
        {
            if(highest_earner->getId() > new_employee->getId()){
                highest_earner = new_employee;
            }
        }
    }
    catch(std::bad_alloc err){
        return MY_ALLOCATION_ERROR;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::RemoveEmployee(int EmployeeID){
    auto employee_iter = all_employees_id_filtered.begin();
    ReturnValue res = all_employees_id_filtered.findElement(EmployeeID, employee_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }
    else{
        Employee* employee_ptr = employee_iter.getData();
        Company* company_ptr = employee_ptr->getCompanyPtr();

        Employee_Key employee_key = Employee_Key(employee_ptr->getId(), employee_ptr->getSalary());
        all_employees_id_filtered.removeElement(EmployeeID);
        all_employees_salary_filtered.removeElement(employee_key);

        company_ptr->removeEmployee(*employee_ptr);

        if(highest_earner == employee_ptr){
            auto rightmost_iter = all_employees_salary_filtered.getRightMost();
            if(rightmost_iter.getPtr() == nullptr){
                highest_earner == nullptr;
            }
            else{
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
    auto company_iter = all_companies.begin();
    ReturnValue res = all_companies.findElement(CompanyID, company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }
    else{
        Company* company_ptr = company_iter.getData();
        if(company_ptr->getSize() > 0){
            return MY_FAILURE;
        }
        else{
            ReturnValue res = all_companies.removeElement(CompanyID);
            if(res!=MY_SUCCESS)
                return res;
        }

        delete company_ptr;
    }

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees){
    auto company_iter = all_companies.begin();
    ReturnValue res = all_companies.findElement(CompanyID, company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    Company* company_ptr = company_iter.getData();
    *Value = company_ptr->getValue();
    *NumEmployees = company_ptr->getSize();

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade){
    auto employee_iter = all_employees_id_filtered.begin();
    ReturnValue res = all_employees_id_filtered.findElement(EmployeeID, employee_iter);
    if(res != ELEMENT_EXISTS){
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
    auto company_iter = all_companies.begin();
    ReturnValue res = all_companies.findElement(CompanyID, company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    Company* company_ptr = company_iter.getData();
    company_ptr->increaseValue(ValueIncrease);
    
    return MY_SUCCESS;
}

ReturnValue CompaniesManager::PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade){
    auto employee_iter = all_employees_id_filtered.begin();
    ReturnValue res = all_employees_id_filtered.findElement(EmployeeID, employee_iter);
    if(res != ELEMENT_EXISTS){
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

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::HireEmployee(int EmployeeID, int NewCompanyID){
    auto employee_iter = all_employees_id_filtered.begin();
    ReturnValue res = all_employees_id_filtered.findElement(EmployeeID, employee_iter);
    if( res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    Employee *employee_ptr = employee_iter.getData();
    Company *curr_company_ptr = employee_ptr->getCompanyPtr();

    if (curr_company_ptr->getId() == NewCompanyID){
        return MY_FAILURE;
    }

    auto new_company_iter = all_companies.begin();
    res = all_companies.findElement(NewCompanyID, new_company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    Company *new_company_ptr = new_company_iter.getData();

    curr_company_ptr->removeEmployee(*employee_ptr);
    employee_ptr->setCompany(new_company_ptr);
    new_company_ptr->addEmployee(*employee_ptr);

    return MY_SUCCESS;
}

ReturnValue CompaniesManager::AcquireCompany(int AcquirerID, int TargetID, double Factor){
    auto acquire_company_iter = all_companies.begin();
    ReturnValue res = all_companies.findElement(AcquirerID, acquire_company_iter);
    if(res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }
    
    auto target_company_iter = all_companies.begin();
    ReturnValue res = all_companies.findElement(TargetID, target_company_iter);
    if(res != ELEMENT_EXISTS){
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

    // need to implement this function!
    updateCompanyForAllEmployees(acquire_company_ptr);
    //

    delete target_company_ptr;
    target_company_ptr = nullptr;

    return MY_SUCCESS;
}

