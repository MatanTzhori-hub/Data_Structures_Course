#include "library2.h"

#include <iostream>

using std::endl;
using std::cout;

 const char* result_array[] = {"SUCCESS", "FAILURE", "ALLOCATION_ERROR", "INVALID_INPUT"};

int main(){
    void* manager = Init(10);
    StatusType res;
    cout << result_array[-1 * AddEmployee(manager, 18, 6, 31)] << endl;
    cout << result_array[-1 * AddEmployee(manager, 1, 2, 4)] << endl;
    cout << result_array[-1 * AddEmployee(manager, 499104, 2, 15)] << endl;
    cout << result_array[-1 * AddEmployee(manager, -2, -2, 33)] << endl;
    cout << result_array[-1 * AddEmployee(manager, 16, 3, 13)] << endl;
    cout << result_array[-1 * AddEmployee(manager, 13, 9, 13)] << endl;
    cout << result_array[-1 * RemoveEmployee(manager, 1)] << endl;
    cout << result_array[-1 * RemoveEmployee(manager, 18)] << endl;
    cout << result_array[-1 * AddEmployee(manager, 3, 10, 39)] << endl;
    cout << result_array[-1 * AcquireCompany(manager, 4, 9 ,1)] << endl;
    cout << result_array[-1 * EmployeeSalaryIncrease(manager, 16, 2)] << endl;
    cout << result_array[-1 * PromoteEmployee(manager, 13, 1)] << endl;
    SumOfBumpGradeBetweenTopWorkersByGroup(manager, 4, 1);
    AverageBumpGradeBetweenSalaryByGroup(manager, 4, -1, 2);
    CompanyValue(manager, 8);
    cout << result_array[-1 * AcquireCompany(manager, 10, 8, 3)] << endl;
    cout << result_array[-1 * AcquireCompany(manager, 8, 7, 4)] << endl;
    cout << result_array[-1 * EmployeeSalaryIncrease(manager, 499104, 6)] << endl;
    cout << result_array[-1 * EmployeeSalaryIncrease(manager, 3, 1)] << endl;
    cout << result_array[-1 * PromoteEmployee(manager, 13, 10)] << endl;
    CompanyValue(manager, 8);
    CompanyValue(manager, 7);
    cout << result_array[-1 * PromoteEmployee(manager, 16, 7)] << endl;
    AverageBumpGradeBetweenSalaryByGroup(manager, 5, 2, 6);
    SumOfBumpGradeBetweenTopWorkersByGroup(manager, 9, 1);
    CompanyValue(manager, 10);
    Quit(&manager);

}


