#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee{
    int employee_id;
    int employee_salary;
    int employee_grade;
    int employees_company_id;

    public:
    Employee(int employee_id, int employee_salary, int employee_grade, int employees_company_id);
    ~Employee();
    

};

#endif  //EMPLOYEE_H