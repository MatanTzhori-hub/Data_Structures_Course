#include "employee.h"

Employee::Employee(int employee_id, int employee_salary, int employee_grade, int company_id):
                    employee_id(employee_id),
                    employee_salary(employee_salary),
                    employee_grade(employee_grade),
                    company_id(company_id){}

int Employee::getCompanyId(){
    return company_id;
}

bool Employee::operator>(Employee &other_employee){
    if(this->employee_salary == other_employee.employee_salary)
        return (this->employee_id < other_employee.employee_id);
    else
        return (this->employee_salary > other_employee.employee_salary);
}

bool Employee::operator<(Employee &other_employee){
    return (other_employee > *this);
}