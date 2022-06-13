#include "employee_rank.h"

EmployeeRank::EmployeeRank(long long grade): weight(1),
									   tree_grade(grade),
							  		   sum_of_grades(grade),
									   grade_bump(0){}


void EmployeeRank::initializeRank(Employee employee){
	weight = 1;
	sum_of_grades = tree_grade;
}

EmployeeRank& EmployeeRank::operator+=(EmployeeRank& other_employee_rank){
	weight += other_employee_rank.weight;
	sum_of_grades += other_employee_rank.sum_of_grades;
	//sum_of_grades += other_employee_rank.grade_bump * other_employee_rank.weight;
	return *this;
}

EmployeeRank& EmployeeRank::operator-=(EmployeeRank& other_employee_rank){
	weight -= other_employee_rank.weight;
	sum_of_grades -= other_employee_rank.sum_of_grades;
	return *this;
}