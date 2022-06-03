#include "employee_rank.h"

EmployeeRank::EmployeeRank(int grade): weight(1),
							  		   sum_of_grades(grade),
									   grade_bump(0){}


void EmployeeRank::initializeRank(Employee employee){
	weight = 1;
	sum_of_grades = employee.getGrade();
	grade_bump = 0;
}

EmployeeRank& EmployeeRank::operator+=(EmployeeRank& other_employee_rank){
	weight += other_employee_rank.weight;
	sum_of_grades += other_employee_rank.sum_of_grades;
	return *this;
}

EmployeeRank& EmployeeRank::operator-=(EmployeeRank& other_employee_rank){
	weight -= other_employee_rank.weight;
	sum_of_grades -= other_employee_rank.sum_of_grades;
	return *this;
}