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

void Company::setValue(int new_value){
    this->company_value = new_value;
}

ReturnValue Company::findEmployee(int employee_id, Employee* returned_employee){
    auto itr = employees_salary_filtered.begin();
    Employee_Key employee_key = Employee_Key(employee_id,0);
    ReturnValue res = employees_salary_filtered.findElement(employee_key, itr);
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
    ReturnValue res = employees_salary_filtered.findElement(employee_key, itr);
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

    if(employees_id_filtered.getSize() == 1){
        setHighestEarner(&employee);
    }
    else{
        if(employee.getSalary() > highest_earner->getSalary()){
            setHighestEarner(&employee);
        }
        else if (employee.getSalary() == highest_earner->getSalary())
        {
            if(employee.getId() < highest_earner->getId()){
                setHighestEarner(&employee);
            }
        }
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

    if(highest_earner == &employee){
            auto rightmost_iter = employees_salary_filtered.getRightMost();
            if(rightmost_iter.getPtr() == nullptr){
                highest_earner == nullptr;
            }
            else{
                highest_earner = rightmost_iter.getData();
            }
        }

    return res;
}

ReturnValue Company::AcquireAnotherCompany(Company* other_company, double Factor){
    int new_value = floor((this->getValue() + other_company->getValue())*Factor);

    Employee* new_highest_earner = nullptr;
    Employee_Key this_key = Employee_Key(this->getHighestEarner()->getId(),this->getHighestEarner()->getId());
    Employee_Key other_key = Employee_Key(other_company->getHighestEarner()->getId(),other_company->getHighestEarner()->getId());
    new_highest_earner = (this_key > other_key) ? this->getHighestEarner() : other_company->getHighestEarner();
    
    employees_id_filtered.mergeToMe(other_company->employees_id_filtered);
    employees_salary_filtered.mergeToMe(other_company->employees_salary_filtered);
    this->setValue(new_value);
    this->setHighestEarner(new_highest_earner);

    return MY_SUCCESS;
}

void Company::updateCompanyForAllEmployees(){
    auto iter = this->employees_id_filtered.begin();
    recursiveUpdateGroupForAllPlayers(iter);
}

void Company::recursiveUpdateGroupForAllPlayers(Iterator<int, Employee*> iter){
    if(iter.getPtr() == nullptr){
		return;
	}
    if(!iter.checkNullLeft()){
        iter.goLeft();
	    recursiveUpdateGroupForAllPlayers(iter);
        iter.goFather();
    }
    if(!iter.checkNullRight()){
        iter.goRight();
	    recursiveUpdateGroupForAllPlayers(iter);
        iter.goFather();
    }
    iter.getData()->setCompany(this);
}