#include "company.h"
#include "tree.h"

Company::Company(int company_id, int company_value) : company_id(company_id), 
                                                      company_value(company_value),
                                                      employees_salary_filtered(),
                                                      employees_id_filtered(),
                                                      highest_earner(nullptr){}

Company::~Company(){
    highest_earner = nullptr;   //TODO: check if we need to call destructures
}

int Company::getSize() const{
    return employees_salary_filtered.getSize();
}

int Company::getId() const{
    return company_id;
}

int Company::getValue() const{
    return company_value;
}

Employee* Company::getHighestEarner() const{
    return highest_earner;
}

void Company::increaseValue(int new_value){
    company_value += new_value;
}

void Company::setHighestEarner(Employee* new_highest_ptr){
    highest_earner = new_highest_ptr;
}

ReturnValue Company::findEmployee(int employee_id, Employee* returned_employee){
    auto itr = employees_salary_filtered.begin();
    Employee_Key employee_key = Employee_Key(employee_id,0);
    ReturnValue res = employees_salary_filtered.pre_order_find_by_key(employee_key, itr);
    if(res == ELEMENT_EXISTS){
        returned_employee = itr.getData();
        return MY_SUCCESS;
    }
    else{
        returned_employee = nullptr;
        return res;
    }
}

bool Company::isEmployeeExist(int employee_id){
    auto itr = employees_salary_filtered.begin();
    Employee_Key employee_key = Employee_Key(employee_id,0);
    ReturnValue res = employees_salary_filtered.pre_order_find_by_key(employee_key, itr);
    if(res == ELEMENT_EXISTS)
        return true;
    else
        return false;
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
    return res;
}

ReturnValue Company::removeEmployee(Employee& employee){
    Employee_Key employee_key = Employee_Key(employee.getId(), employee.getSalary());
    ReturnValue res = employees_salary_filtered.removeElement(employee_key);
    if (res != MY_SUCCESS){
        return res;
    }
    res = employees_id_filtered.removeElement(employee.getId());
    return res;
}