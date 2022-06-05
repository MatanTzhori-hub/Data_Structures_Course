#include "company.h"
#include "employee.h"

#include <iostream>

using std::endl;

int main(){

    double avg_grades = 0;

    Company comp1(1, 2);

    Employee emp1(1, 0, 1, &comp1);
    Employee emp2(2, 0, 2, &comp1);
    Employee emp3(10, 0, 3, &comp1);
    Employee emp4(5, 0, 4, &comp1);
    Employee emp5(7, 0, 5, &comp1);
    Employee emp6(3, 0, 6, &comp1);
    Employee emp7(11, 0, 7, &comp1);
    Employee emp8(17, 0, 9, &comp1);
    Employee emp9(299, 0, 3, &comp1);
    Employee emp10(173, 0, 5, &comp1);
    Employee emp11(19, 0, 5, &comp1);
    Employee emp12(189, 0, 6, &comp1);
    Employee emp13(79, 0, 4, &comp1);
    Employee emp14(209, 0, 1, &comp1);
    Employee emp15(156, 0, 2, &comp1);
    Employee emp16(80, 0, 8, &comp1);
    Employee emp17(43, 0, 7, &comp1);
    Employee emp18(91, 0, 2, &comp1);
    Employee emp19(46, 0, 0, &comp1);
    Employee emp20(100, 0, 3, &comp1);
    Employee emp21(47, 0, 1, &comp1);

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

    comp1.bumpGradeInRange(50, 100, 2);
    comp1.bumpGradeInRange(0, 100, 2);

    comp1.addEmployee(emp11);
    comp1.addEmployee(emp12);
    comp1.addEmployee(emp13);
    comp1.addEmployee(emp14);
    comp1.addEmployee(emp15);

    comp1.bumpGradeInRange(0, 100, 3);

    comp1.addEmployee(emp16);
    comp1.addEmployee(emp17);
    comp1.addEmployee(emp18);
    comp1.addEmployee(emp19);
    comp1.addEmployee(emp20);
    comp1.addEmployee(emp21);


    avg_grades = comp1.calcAvgGradeInRange(50, 100);
    cout << avg_grades << endl;
    avg_grades = comp1.calcAvgGradeInRange(0, 100);
    cout << avg_grades << endl;

    comp1.removeEmployee(emp11.getId());
    comp1.removeEmployee(emp12.getId());
    comp1.removeEmployee(emp13.getId());
    comp1.removeEmployee(emp14.getId());
    comp1.removeEmployee(emp15.getId());

    avg_grades = comp1.calcAvgGradeInRange(50, 100);
    cout << avg_grades << endl;
    avg_grades = comp1.calcAvgGradeInRange(0, 100);
    cout << avg_grades << endl;


    comp1.employeeSalaryIncrease(7, 1);
    comp1.employeeSalaryIncrease(2, 50);
    comp1.employeeSalaryIncrease(4, 30);
    comp1.employeeSalaryIncrease(3, 50);
    comp1.employeeSalaryIncrease(11, 70);
    comp1.employeeSalaryIncrease(2, 40);
    comp1.employeeSalaryIncrease(7, 50);
    comp1.employeeSalaryIncrease(19, 5);
    comp1.employeeSalaryIncrease(80, 104);
    comp1.employeeSalaryIncrease(7, 18);
    comp1.employeeSalaryIncrease(91, 400);

    avg_grades = comp1.calcAvgGradeInRange(0, 500);
    cout << avg_grades << endl;

    return 0;
}
