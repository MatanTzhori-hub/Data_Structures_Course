#include "employee_key.h"

Employee_Key::Employee_Key(int employee_id, int employee_salary): employee_id(employee_id), 
                                                        employee_salary(employee_salary){};

bool Employee_Key::operator>(const Employee_Key& other) const{
            if(this->employee_salary == other.employee_salary)
                return this->employee_id < other.employee_id;
            else
                return this->employee_salary > other.employee_salary;
        }

bool Employee_Key::operator<(const Employee_Key& other) const{
            return other > *this;
    }

bool operator==(const Employee_Key& first, const Employee_Key& second){
    return first.getId() == second.getId();
}

bool operator==(const Employee_Key& employee_key, int id_to_compare){
    return employee_key.getId() == id_to_compare;
}

bool operator==(int id_to_compare, const Employee_Key& employee_key){
    return employee_key.getId() == id_to_compare;
}

bool operator!=(const Employee_Key& first, const Employee_Key& second){
    return !(first == second);
}