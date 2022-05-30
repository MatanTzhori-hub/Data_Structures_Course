#include "company.h"

Company::Company(int company_id, int company_value) : company_id(company_id), 
                                                      company_value(company_value),
                                                      employee_hash_table(),
                                                      zero_salary_employees_list(),
                                                      employees_tree_salary_filtered(){}

Company::~Company(){
}

int Company::getSize() const{
    return zero_salary_employees_list.getSize() + employees_tree_salary_filtered.getSize();
}

int Company::getId() const{
    return company_id;
}

int Company::getValue() const{
    return company_value;
}

void Company::increaseValue(int value_increase){
    company_value += value_increase;
}

void Company::setValue(int new_value){
    this->company_value = new_value;
}

ReturnValue Company::findEmployee(int employee_id, Employee* returned_employee){
    Employee temp_employee(employee_id, 0, 0, 0);
    EmployeeHashtableVal temp_employee_hash_val()
    employee_hash_table.findIndex(employee_id);
    if(iter != employees_id_filtered.end()){
        returned_employee = iter.getData();
        return MY_SUCCESS;
    }
    else{
        returned_employee = nullptr;
        return MY_FAILURE;
    }
}

bool Company::isEmployeeExist(int employee_id){
    auto iter = employees_id_filtered.findElement(employee_id);
    if(iter != employees_id_filtered.end()){
        return true;
    }
    else{
        return false;
    }
}

ReturnValue Company::addEmployee(Employee& employee){
    Employee_Key employee_key = Employee_Key(employee.getId(), employee.getSalary());
    ReturnValue res = employees_salary_filtered.insert(employee_key, &employee);
    if (res != MY_SUCCESS){
        return res;
    }
    res = employees_id_filtered.insert(employee.getId(), &employee);
    if (res != MY_SUCCESS){
        employees_salary_filtered.removeElement(employee_key);
        return res;
    }

    // Check if need to update highest_earner
    if(employees_salary_filtered.getSize() == 1){
        highest_earner = &employee;
    }
    else if(*highest_earner < employee){
        highest_earner = &employee;
    }

    return res;
}

ReturnValue Company::removeEmployee(Employee& employee){
    Employee_Key employee_key = Employee_Key(employee.getId(), employee.getSalary());
    ReturnValue res = employees_salary_filtered.removeElement(employee_key);
    if (res != MY_SUCCESS){
        return res;
    }
    res = employees_id_filtered.removeElement(employee.getId());

    // Check if need to update highest_earner
    if(highest_earner == &employee){
        highest_earner = nullptr;
        if(employees_id_filtered.getSize() != 0){
            Iterator<Employee_Key, Employee*> rightmost_iter = employees_salary_filtered.getRightMost();
            highest_earner = rightmost_iter.getData();
        }
    }

    return res;
}

ReturnValue Company::AcquireAnotherCompany(Company* other_company, double Factor){
    int new_value = floor((this->getValue() + other_company->getValue())*Factor);

    Employee* new_highest_earner = this->getHighestEarner();
    if(other_company->getSize() > 0){
        if(this->getSize() == 0){
            new_highest_earner = other_company->getHighestEarner();
        }
        else{
            if(*new_highest_earner < *(other_company->getHighestEarner())){
                new_highest_earner = other_company->getHighestEarner();
            }
        }

        employees_id_filtered.mergeToMe(other_company->employees_id_filtered);
        employees_salary_filtered.mergeToMe(other_company->employees_salary_filtered);
    }
    this->setValue(new_value);
    this->setHighestEarner(new_highest_earner);

    return MY_SUCCESS;
}

void Company::updateCompanyForAllEmployees(){
    auto iter = this->employees_id_filtered.begin();
    while(iter != employees_id_filtered.end()){
        iter.getData()->setCompany(this);
        iter.next();
    }
}


ReturnValue Company::GetAllEmployeesBySalary(int **Employees, int *NumOfEmployees){
    *NumOfEmployees = employees_salary_filtered.getSize();
    if(*NumOfEmployees <= 0){
        return MY_FAILURE;
    }
    else{
        int *Employees_arr = (int*)malloc(*NumOfEmployees * sizeof(int));
        if(Employees_arr == NULL){
            return MY_ALLOCATION_ERROR;
        }
        auto employee_iter = employees_salary_filtered.begin(-1);

        for(int i = 0; i < *NumOfEmployees; i++){
            Employees_arr[i] = employee_iter.getData()->getId();
            employee_iter.next();
        }

        *Employees = Employees_arr;
    }
    return MY_SUCCESS;
}

ReturnValue Company::GetNumEmployeesMatching(int MinEmployeeID, int MaxEmployeeId, int MinSalary, 
                                            int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees){
    if(employees_id_filtered.getSize() == 0){
        return MY_FAILURE;
    }

    int numEmployees = 0;
    int totalNumEmployees = 0;
    auto start_iter = employees_id_filtered.findCloseestElement(MinEmployeeID);
    auto end_iter = employees_id_filtered.findCloseestElement(MaxEmployeeId);

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

    // once more, incase end_iter is the largest key and still in range.
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
