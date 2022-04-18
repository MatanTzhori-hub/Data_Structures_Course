#include "employee_key.h"

Employee_Key::Employee_Key(int employee_id, int employee_salary): employee_id(employee_id), 
                                                        employee_salary(employee_salary){};

bool Employee_Key::operator>(Employee_Key& other){
            if(this->employee_salary == other.employee_salary)
                return this->employee_id > other.employee_id;
            else
                return this->employee_salary > other.employee_salary;
        }

bool Employee_Key::operator<(Employee_Key& other){
            return other.employee_salary > this->employee_salary;
    }