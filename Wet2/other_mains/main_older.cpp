#include "company.h"
#include "employee.h"
#include "companies_manager.h"

#include <iostream>

using std::endl;

int main(){

    double avg_grades = 0;
    int sum_top = 0;

    Company comp1(1, 2);

    Employee emp1(1, 0, 1, &comp1);
    Employee emp2(2, 0, 2, &comp1);
    Employee emp3(3, 0, 3, &comp1);
    Employee emp4(5, 0, 4, &comp1);
    Employee emp5(7, 0, 5, &comp1);
    Employee emp6(10, 0, 6, &comp1);
    Employee emp7(11, 0, 7, &comp1);
    Employee emp8(17, 0, 9, &comp1);
    Employee emp9(19, 0, 3, &comp1);
    Employee emp10(43, 0, 5, &comp1);
    Employee emp11(46, 0, 5, &comp1);
    Employee emp12(47, 0, 6, &comp1);
    Employee emp13(79, 0, 4, &comp1);
    Employee emp14(80, 0, 1, &comp1);
    Employee emp15(91, 0, 2, &comp1);
    Employee emp16(100, 0, 8, &comp1);
    Employee emp17(156, 0, 7, &comp1);
    Employee emp18(173, 0, 2, &comp1);
    Employee emp19(189, 0, 0, &comp1);
    Employee emp20(209, 0, 3, &comp1);
    Employee emp21(299, 0, 1, &comp1);

    comp1.addEmployee(emp1);
    comp1.addEmployee(emp2);
    comp1.addEmployee(emp3);
    comp1.addEmployee(emp4);
    comp1.addEmployee(emp5);
    comp1.addEmployee(emp6);
    comp1.addEmployee(emp7);
    comp1.addEmployee(emp8);
    comp1.addEmployee(emp9);
    comp1.addEmployee(emp10);
    comp1.addEmployee(emp11);
    comp1.addEmployee(emp12);
    comp1.addEmployee(emp13);
    comp1.addEmployee(emp14);
    comp1.addEmployee(emp15);
    comp1.addEmployee(emp16);
    comp1.addEmployee(emp17);
    comp1.addEmployee(emp18);
    comp1.addEmployee(emp19);
    comp1.addEmployee(emp20);
    comp1.addEmployee(emp21);


    comp1.employeeSalaryIncrease(2, 90);
    comp1.employeeSalaryIncrease(3, 50);
    comp1.employeeSalaryIncrease(7, 69);
    comp1.employeeSalaryIncrease(11, 70);
    comp1.employeeSalaryIncrease(43, 70);
    comp1.employeeSalaryIncrease(46, 70);
    comp1.employeeSalaryIncrease(47, 70);
    comp1.employeeSalaryIncrease(80, 104);  //36 + 4 + 6 + 12 + 4 + 3     65
    comp1.employeeSalaryIncrease(91, 400);
    // top 3 = 5
    
    comp1.bumpGradeInRange(50, 69, 2);
    // top 3 = 5

    comp1.bumpGradeInRange(101, 405, 3);
    // top 3 = 11

    comp1.employeeSalaryIncrease(17, 80);
    comp1.employeeSalaryIncrease(19, 85);
    // top 3 = 11

    comp1.bumpGradeInRange(70, 70, 1);
    // top 3 = 11

    comp1.employeeSalaryIncrease(209, 105);
    // top 3 = 12
    comp1.employeeSalaryIncrease(299, 106);
    // top 3 = 9

    //avg_grades = comp1.calcAvgGradeInRange(1, 500);
    cout << avg_grades << endl;

    sum_top = comp1.calcSumGradeOfmTop(6);
    cout << sum_top << endl;
    sum_top = comp1.calcSumGradeOfmTop(8);
    cout << sum_top << endl;
    sum_top = comp1.calcSumGradeOfmTop(12);
    cout << sum_top << endl;
    sum_top = comp1.calcSumGradeOfmTop(13);
    cout << sum_top << endl;

    comp1.removeEmployee(11);
    comp1.removeEmployee(43);

    sum_top = comp1.calcSumGradeOfmTop(3);
    cout << sum_top << endl;

    return 0;
}
