#include "employee.h"

Employee::Employee(int employee_id, int employee_salary, int employee_grade, Company* company_ptr):
                    employee_id(employee_id),
                    employee_salary(employee_salary),
                    employee_grade(employee_grade),
                    company_ptr(company_ptr){}

int Employee::getCompanyId(){
    return company_ptr->getId(); 
}