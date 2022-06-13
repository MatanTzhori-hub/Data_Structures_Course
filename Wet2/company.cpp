#include "company.h"

Company::Company(int company_id, double company_value) : company_id(company_id), 
                                                      company_value(company_value),
                                                      employee_hash_table(),
                                                      zero_salary_employees_list(),
                                                      employees_tree_salary_filtered(){}


// todo : implement this!
void Company::killAllEmployees(){
    employee_hash_table.killAllEmployees();
}

int Company::getSize() const{
    return zero_salary_employees_list.getSize() + employees_tree_salary_filtered.getSize();
}

int Company::getPayedEmployeesNum(){
    return employees_tree_salary_filtered.getSize();
}

int Company::getId() const{
    return company_id;
}

double Company::getValue() const{
    return company_value;
}

void Company::increaseValue(double value_increase){
    company_value += value_increase;
}

void Company::setValue(double new_value){
    this->company_value = new_value;
}

Employee* Company::findEmployee(int employee_id,  ReturnValue* res){
    ReturnValue ret = MY_FAILURE;
    EmployeeHashtableVal* temp_employee_hash_val = employee_hash_table.findElement(employee_id, &ret);
    
    if(ret != ELEMENT_EXISTS){
        *res = ret;
        return nullptr;
    }
    else{
        *res = ret;
        return temp_employee_hash_val->getEmployeePtr();
    }
}


Node<EmployeeKey,Employee*,EmployeeRank>* Company::getEmployeeTreeNode(int employee_id, ReturnValue* res){
    ReturnValue ret = MY_FAILURE;
    EmployeeHashtableVal* temp_employee_hash_val = employee_hash_table.findElement(employee_id, &ret);
    
    if(ret != ELEMENT_EXISTS){
        *res = ret;
        return nullptr;
    }
    else{
        *res = ret;
        return temp_employee_hash_val->getTreeNode();
    }
}


bool Company::isEmployeeExist(int employee_id){
    ReturnValue ret = MY_FAILURE;
    EmployeeHashtableVal* temp_val = employee_hash_table.findElement(employee_id, &ret);
    
    if(ret == ELEMENT_EXISTS){
        return true;
    }
    return false;
}

ReturnValue Company::addEmployee(Employee& employee){
    EmployeeKey employee_key = EmployeeKey(employee.getId(), employee.getSalary());
    ReturnValue res = MY_FAILURE;
    EmployeeHashtableVal* temp_val = employee_hash_table.findElement(employee.getId(), &res);
    
    if (res == ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    EmployeeHashtableVal new_val(&employee);
    if (employee.getSalary() == 0){
        res = zero_salary_employees_list.insert(&employee);
        if (res != MY_SUCCESS){
            return res;
        }
        new_val.setListNode(zero_salary_employees_list.getHead());
        res = employee_hash_table.insertElement(new_val, employee.getId());
        if (res != MY_SUCCESS){
            zero_salary_employees_list.remove(zero_salary_employees_list.getHead());
            return res;
        }
    } else {
        EmployeeKey new_key(employee.getId(), employee.getSalary());
        EmployeeRank new_rank(employee.getGrade());
        res = employees_tree_salary_filtered.insert(new_key, &employee, new_rank);
        if (res != MY_SUCCESS){
            return res;
        }

        new_val.setTreeNode(employees_tree_salary_filtered.findElementNode(new_key));
        res = employee_hash_table.insertElement(new_val, employee.getId());
        if (res != MY_SUCCESS){
            employees_tree_salary_filtered.removeElement(new_key);
            return res;
        }
    }
    return MY_SUCCESS;
}

ReturnValue Company::removeEmployee(int employee_id){
    ReturnValue res = MY_FAILURE;
    EmployeeHashtableVal* hash_val = employee_hash_table.findElement(employee_id, &res);
    
    if (res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    if (hash_val->getTreeNode() == nullptr){
        // remove from list
        DoublyLinkedListNode<Employee*>* temp_node_ptr = hash_val->getListNode();
        res = zero_salary_employees_list.remove(temp_node_ptr);
        if (res != MY_SUCCESS){
            return MY_FAILURE;
        }
        hash_val->setListNode(nullptr);
    } else if (hash_val->getListNode() == nullptr){
        // remove from tree
        res = employees_tree_salary_filtered.removeElement(hash_val->getTreeNode()->getKey());
        if (res != MY_SUCCESS){
            return MY_FAILURE;
        }
        hash_val->setTreeNode(nullptr);
    }

    // remove from hashtable
    res = employee_hash_table.removeElement(employee_id);
    return res;
}

ReturnValue Company::AcquireAnotherCompany(Company* other_company){
    // merge lists
    ReturnValue res = zero_salary_employees_list.merge_to_me(other_company->zero_salary_employees_list);
    if (res != MY_SUCCESS){
        return res;
    }

    // merge trees
    employees_tree_salary_filtered.mergeToMe(other_company->employees_tree_salary_filtered);

    // merge hashtables
    employee_hash_table.mergeToMe(&(other_company->employee_hash_table));

    // scan new combined tree and update hash table
    Iterator<EmployeeKey, Employee*, EmployeeRank> iter = employees_tree_salary_filtered.begin();

    res = MY_FAILURE;
    EmployeeHashtableVal* temp_hash_val;

    while (iter != employees_tree_salary_filtered.end()){
        temp_hash_val = employee_hash_table.findElement(iter.getKey().getId(), &res);
        temp_hash_val->setTreeNode(iter.getNodePtr());
        iter.next();
    }

    updateCompanyForAllEmployees();

    return MY_SUCCESS;
}

void Company::updateCompanyForAllEmployees(){
    // update company for employees in tree
    Iterator<EmployeeKey, Employee*, EmployeeRank> tree_iter = employees_tree_salary_filtered.begin();
    while (tree_iter != employees_tree_salary_filtered.end()){
        tree_iter.getData()->setCompany(this);
        tree_iter.next();
    }

    // update company for employees in list
    DoublyLinkedListNode<Employee*>* list_iter_ptr = zero_salary_employees_list.getHead();
    while (list_iter_ptr){
        list_iter_ptr->getData()->setCompany(this);
        list_iter_ptr = list_iter_ptr->getNext();
    }
}

ReturnValue Company::employeeSalaryIncrease(int employee_id, int salary_increase){
    ReturnValue res = MY_FAILURE;
    EmployeeHashtableVal* employee_val = employee_hash_table.findElement(employee_id, &res);
    
    if(res != ELEMENT_EXISTS){
        return res;
    }
    else{
        Employee* employee = employee_val->getEmployeePtr();

        // if true, then employee has salary > 0.
        if(employee_val->getListNode() == nullptr){
            EmployeeKey temp_key = EmployeeKey(employee_id, employee->getSalary());
            employees_tree_salary_filtered.removeElement(temp_key);
            
        }
        // if true, then employee has salary = 0.
        else if(employee_val->getTreeNode() == nullptr){
            zero_salary_employees_list.remove(employee_val->getListNode());
        }

        employee->setSalary(employee->getSalary() + salary_increase);
        EmployeeKey new_key = EmployeeKey(employee_id, employee->getSalary());
        EmployeeRank new_rank = EmployeeRank(employee->getGrade());
        employees_tree_salary_filtered.insert(new_key, employee, new_rank);
        Node<EmployeeKey, Employee*, EmployeeRank>* new_node_in_tree = employees_tree_salary_filtered.findElementNode(new_key);
        employee_val->setTreeNode(new_node_in_tree);
        employee_val->setNullListNode();

        return MY_SUCCESS;
    }
}

long long Company::calcSumGradeInRange(int lowest_salary, int highest_salary) {
    long long sum_grades_in_range = 0;

    if(lowest_salary > 0){
        if(employees_tree_salary_filtered.getSize() == 0)
            return 0;
    }
    if(highest_salary <= 0){
        if(zero_salary_employees_list.getSize() == 0)
            return 0;
    }

    Employee* highest_salary_employee;
    Employee* lowest_salary_employee;

    if (employees_tree_salary_filtered.getSize() == 0) {
        highest_salary_employee = zero_salary_employees_list.getTail()->getData();
    } else {
        highest_salary_employee = employees_tree_salary_filtered.getRightMost().getData();
    }

    if (zero_salary_employees_list.getSize() == 0) {
        lowest_salary_employee = employees_tree_salary_filtered.begin().getData();
    } else {
        lowest_salary_employee = zero_salary_employees_list.getHead()->getData();
    }

    if (lowest_salary > highest_salary_employee->getSalary() || highest_salary < lowest_salary_employee->getSalary()){
        return 0;
    }

    bool list_included = (lowest_salary <= 0 && highest_salary >= 0);
    bool tree_included = (lowest_salary > 0 || highest_salary > 0);

    // if 0 is included in range (list_included) get num of players with salary 0
    if (list_included) {
        sum_grades_in_range += zero_salary_employees_list.getGradeSum();
    }

    // if tree is included (salaries 1 and up are in range)
    if (tree_included) {
        sum_grades_in_range += employees_tree_salary_filtered.getSumGradeInRange(lowest_salary, highest_salary);
    }

    return sum_grades_in_range;
}

int Company::calcNumEmployeesInRange(int lowest_salary, int highest_salary){
    int sum_employees_in_range = 0;

    if(lowest_salary > 0){
        if(employees_tree_salary_filtered.getSize() == 0)
            return 0;
    }
    if(highest_salary <= 0){
        if(zero_salary_employees_list.getSize() == 0)
            return 0;
    }

    Employee* highest_salary_employee;
    Employee* lowest_salary_employee;

    if (employees_tree_salary_filtered.getSize() == 0) {
        highest_salary_employee = zero_salary_employees_list.getTail()->getData();
    } else {
        highest_salary_employee = employees_tree_salary_filtered.getRightMost().getData();
    }

    if (zero_salary_employees_list.getSize() == 0) {
        lowest_salary_employee = employees_tree_salary_filtered.begin().getData();
    } else {
        lowest_salary_employee = zero_salary_employees_list.getHead()->getData();
    }

    if (lowest_salary > highest_salary_employee->getSalary() || highest_salary < lowest_salary_employee->getSalary()){
        return 0;
    }

    bool list_included = (lowest_salary <= 0 && highest_salary >= 0);
    bool tree_included = (lowest_salary > 0 || highest_salary > 0);

    // if 0 is included in range (list_included) get num of players with salary 0
    if (list_included) {
        sum_employees_in_range += zero_salary_employees_list.getSize();
    }

    // if tree is included (salaries 1 and up are in range)
    if (tree_included) {
        sum_employees_in_range += employees_tree_salary_filtered.getNumEmployeesInRange(lowest_salary, highest_salary);
    }

    return sum_employees_in_range;
}


ReturnValue Company::calcAvgGradeInRange(int lowest_salary, int highest_salary, double *avg){
    if((zero_salary_employees_list.getSize() + employees_tree_salary_filtered.getSize()) == 0){
        *avg = 0;
        return MY_FAILURE;
    }

    int sum_employees = calcNumEmployeesInRange(lowest_salary, highest_salary);
    if(sum_employees == 0){
        *avg = 0;
        return MY_FAILURE;
    }

    long long sum_grade = calcSumGradeInRange(lowest_salary, highest_salary);
    *avg = (double)sum_grade / sum_employees;
    
    return MY_SUCCESS;
}


long long Company::calcSumGradeOfmTop(int m){
    if(m > employees_tree_salary_filtered.getSize()){
        return -1;
    }
    else if(m == employees_tree_salary_filtered.getSize()){
        return employees_tree_salary_filtered.getTotalSumGrade();
    }
    else{
        return employees_tree_salary_filtered.getSumGradeOfmTop(m);
    }
}


ReturnValue Company::promoteEmployee(int employee_id, long long bump_amount){
    ReturnValue res = MY_FAILURE;
    EmployeeHashtableVal* emp_val = employee_hash_table.findElement(employee_id, &res);

    if(res == ELEMENT_DOES_NOT_EXIST){
        return MY_FAILURE;
    }
    else if(bump_amount <= 0){
        return MY_SUCCESS;
    }

    if(emp_val->getTreeNode() == nullptr){
        zero_salary_employees_list.bumpSingleEmployeeGrade(emp_val->getListNode(), bump_amount);
        // because we increase the employee grade in both the real company and company_0, we want to increase the real grade only once
        if(this->company_id == 0){
            emp_val->getEmployeePtr()->setGrade(emp_val->getEmployeePtr()->getGrade() - bump_amount);
        }
    }
    else if(emp_val->getListNode() == nullptr){
        employees_tree_salary_filtered.bumpSingleEmployeeGrade(emp_val->getTreeNode(), bump_amount);
    }
    else{
        return MY_FAILURE;
    }

    return MY_SUCCESS;
}


void Company::bumpGradeInRange(int lowest_salary, int highest_salary, long long bump_amount){
    if(lowest_salary > 0){
        if(employees_tree_salary_filtered.getSize() == 0)
            return;
    }
    if(highest_salary <= 0){
        if(zero_salary_employees_list.getSize() == 0)
            return;
    }

    Employee* highest_salary_employee;
    Employee* lowest_salary_employee;

    if (employees_tree_salary_filtered.getSize() == 0) {
        highest_salary_employee = zero_salary_employees_list.getTail()->getData();
    } else {
        highest_salary_employee = employees_tree_salary_filtered.getRightMost().getData();
    }

    if (zero_salary_employees_list.getSize() == 0) {
        lowest_salary_employee = employees_tree_salary_filtered.begin().getData();
    } else {
        lowest_salary_employee = zero_salary_employees_list.getHead()->getData();
    }

    if (lowest_salary > highest_salary_employee->getSalary() || highest_salary < lowest_salary_employee->getSalary()){
        return;
    }

    bool list_included = (lowest_salary <= 0 && highest_salary >= 0);
    bool tree_included = (lowest_salary > 0 || highest_salary > 0);

    // if 0 is included in range (list_included) get num of players with salary 0
    if (list_included) {
        zero_salary_employees_list.bumpAllGrade(bump_amount);
    }

    // if tree is included (salaries 1 and up are in range)
    if (tree_included) {
        employees_tree_salary_filtered.bumpGradeInRange(lowest_salary, highest_salary, bump_amount);
    }
}

Company& Company::operator+=(Company& other){
    this->AcquireAnotherCompany(&other);
    return *this;
}