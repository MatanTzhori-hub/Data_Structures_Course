#ifndef COMPANY_H
#define COMPANY_H

#include "rank_tree.h"
#include "doubly_linked_list.h"
#include "dynamic_hash_table.h"
#include "employee_key.h"
#include "employee.h"
#include "employee_rank.h"
#include "employee_hashtable_val.h"

class Employee;
class EmployeeHashtableVal;
class EmployeeRank;

class Company{
    int company_id;
    int company_value;
    DynamicHashTable<EmployeeHashtableVal> employee_hash_table;
    DoublyLinkedList<Employee*> zero_salary_employees_list;
    RankTree<EmployeeKey,Employee*,EmployeeRank> employees_tree_salary_filtered;
    
    int calcSumGradeInRange(int lowest_salary, int highest_salary);
    int calcNumEmployeesInRange(int lowest_salary, int highest_salary);

    public:
    Company(int company_id, int company_value);
    ~Company() = default;
    void killAllEmployees();

    int getSize() const;
    int getPayedEmployeesNum();
    int getId() const;
    int getValue() const;
    void increaseValue(int new_value);
    void setValue(int new_value);
    Employee* findEmployee(int employee_id, ReturnValue* res);
    bool isEmployeeExist(int employee_id);
    ReturnValue addEmployee(Employee& employee);
    ReturnValue removeEmployee(int employee_id);
    ReturnValue AcquireAnotherCompany(Company* other);
    void updateCompanyForAllEmployees();
    ReturnValue employeeSalaryIncrease(int employee_id, int salary_increase);
    
    ReturnValue calcAvgGradeInRange(int lowest_salary, int highest_salary, double *avg);
    int calcSumGradeOfmTop(int m);

    // todo : imlement the following 2 functions
    ReturnValue promoteEmployee(int employee_id, int bump_amount);
    void bumpGradeInRange(int lowest_salary, int highest_salary, int bump_amount);

    Company& operator+=(Company& other);
};

#endif // COMPANY_H