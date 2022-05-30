#include "company.h"

Company::Company(int company_id, int company_value) : company_id(company_id), 
                                                      company_value(company_value),
                                                      employee_hash_table(),
                                                      zero_salary_employees_list(),
                                                      employees_tree_salary_filtered(){}

Company::~Company(){
}

int Company::getSize() const{
    return zero_salary_employees_list.getSize() + employees_tree_salary_filtered.getSize();
}

int Company::getId() const{
    return company_id;
}

int Company::getValue() const{
    return company_value;
}

void Company::increaseValue(int value_increase){
    company_value += value_increase;
}

void Company::setValue(int new_value){
    this->company_value = new_value;
}

ReturnValue Company::findEmployee(int employee_id, Employee* returned_employee){
    Employee temp_employee(employee_id, 0, 0, 0);
    EmployeeHashtableVal temp_employee_hash_val = EmployeeHashtableVal();
    ReturnValue ret = employee_hash_table.findElement(employee_id, &temp_employee_hash_val);
    if(ret != MY_SUCCESS){
        return MY_FAILURE;
    }
    else{
        returned_employee = temp_employee_hash_val.getEmployeePtr();
        return MY_SUCCESS;
    }
}

bool Company::isEmployeeExist(int employee_id){
    EmployeeHashtableVal temp_val;
    ReturnValue ret = employee_hash_table.findElement(employee_id, &temp_val);
    if(ret == ELEMENT_EXISTS){
        return true;
    }
    return false;
}

ReturnValue Company::addEmployee(Employee& employee){
    EmployeeKey employee_key = EmployeeKey(employee.getId(), employee.getSalary());
    EmployeeHashtableVal new_val;
    ReturnValue res = employee_hash_table.findElement(employee.getId(), &new_val);
    if (res == ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    new_val.setEmployeePtr(&employee);
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
    EmployeeHashtableVal hash_val;
    ReturnValue res = employee_hash_table.findElement(employee_id, &hash_val);
    if (res != ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    if (hash_val.getTreeNode() == nullptr){
        // remove from list
        DoublyLinkedListNode<Employee*>* temp_node_ptr = hash_val.getListNode();
        res = zero_salary_employees_list.remove(temp_node_ptr);
        if (res != MY_SUCCESS){
            return MY_FAILURE;
        }
        hash_val.setListNode(nullptr);
    } else if (hash_val.getListNode() == nullptr){
        // remove from tree
        res = employees_tree_salary_filtered.removeElement(hash_val.getTreeNode()->getKey());
        if (res != MY_SUCCESS){
            return MY_FAILURE;
        }
        hash_val.setTreeNode(nullptr);
    }

    // remove from hashtable
    res = employee_hash_table.removeElement(employee_id);
    return res;
}

ReturnValue Company::AcquireAnotherCompany(Company* other_company, double Factor){
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
    EmployeeHashtableVal temp_hash_val;

    while (iter != employees_tree_salary_filtered.end()){
        employee_hash_table.findElement(iter.getKey().getId(), &temp_hash_val);
        temp_hash_val.setTreeNode(iter.getNodePtr());
    }
}

void Company::updateCompanyForAllEmployees(){
    // update company for employees in tree
    Iterator<EmployeeKey, Employee*, EmployeeRank> tree_iter = employees_tree_salary_filtered.begin();
    while (tree_iter != employees_tree_salary_filtered.end()){
        tree_iter.getData()->setCompany(company_id);
    }

    // update company for employees in list
    DoublyLinkedListNode<Employee*>* list_iter_ptr = zero_salary_employees_list.getHead();
    while (list_iter_ptr){
        list_iter_ptr->getData()->setCompany(company_id);
        list_iter_ptr = list_iter_ptr->getNext();
    }
}

// todo: implement functions for:
//       (1) calculating avg grade for all employees (in lower/higher range)
//       (2) calculating sum_grades for top m employees (in tree - use rank tree trick from tutorial 6)

/*

ReturnValue Company::GetNumEmployeesMatching(int MinEmployeeID, int MaxEmployeeId, int MinSalary,
                                            int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees){
    if(employees_id_filtered.getSize() == 0){
        return MY_FAILURE;
    }

    int numEmployees = 0;
    int totalNumEmployees = 0;
    auto start_iter = employees_id_filtered.findCloseestElement(MinEmployeeID);
    auto end_iter = employees_id_filtered.findCloseestElement(MaxEmployeeId);

    while(start_iter != end_iter){
        Employee* employee_ptr = start_iter.getData();
        if( MinEmployeeID <= employee_ptr->getId() && employee_ptr->getId() <= MaxEmployeeId){
            totalNumEmployees++;
            if(MinSalary <= employee_ptr->getSalary() && MinGrade <= employee_ptr->getGrade()){
                numEmployees++;
            }
        }
        start_iter.next();
    }

    // once more, incase end_iter is the largest key and still in range.
    Employee* employee_ptr = start_iter.getData();
    if( MinEmployeeID <= employee_ptr->getId() && employee_ptr->getId() <= MaxEmployeeId){
        totalNumEmployees++;
        if(MinSalary <= employee_ptr->getSalary() && MinGrade <= employee_ptr->getGrade()){
            numEmployees++;
        }
    }

    *TotalNumOfEmployees = totalNumEmployees;
    *NumOfEmployees = numEmployees;
    return MY_SUCCESS;
}
*/

