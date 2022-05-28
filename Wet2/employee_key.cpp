#include "employee_key.h"

EmployeeKey::EmployeeKey(int employee_id, int employee_salary): employee_id(employee_id), 
                                                        employee_salary(employee_salary){};

bool EmployeeKey::operator>(const EmployeeKey& other) const{
            if(this->employee_salary == other.employee_salary)
                return this->employee_id < other.employee_id;
            else
                return this->employee_salary > other.employee_salary;
        }

bool EmployeeKey::operator<(const EmployeeKey& other) const{
            return other > *this;
    }

bool operator==(const EmployeeKey& first, const EmployeeKey& second){
    return first.getId() == second.getId();
}

bool operator==(const EmployeeKey& employee_key, int id_to_compare){
    return employee_key.getId() == id_to_compare;
}

bool operator==(int id_to_compare, const EmployeeKey& employee_key){
    return employee_key.getId() == id_to_compare;
}

bool operator!=(const EmployeeKey& first, const EmployeeKey& second){
    return !(first == second);
}