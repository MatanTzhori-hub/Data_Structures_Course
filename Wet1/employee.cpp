#include "employee.h"

Employee::Employee(int employee_id, int employee_salary, int employee_grade, int company_id):
                    employee_id(employee_id),
                    employee_salary(employee_salary),
                    employee_grade(employee_grade),
                    company_id(company_id){}

Employee Employee::operator=(Employee other_employee){
    this->setCompany(other_employee.company_id);
    this->setCompany(other_employee.company_id);
}