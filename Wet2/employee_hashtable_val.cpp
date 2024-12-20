#include "employee_hashtable_val.h"


EmployeeHashtableVal::~EmployeeHashtableVal() {
    employee_ptr = nullptr;
    tree_node = nullptr;
    list_node = nullptr;
}

void EmployeeHashtableVal::initializeVal(){
    employee_ptr = nullptr;
    tree_node = nullptr;
    list_node = nullptr;
}

int EmployeeHashtableVal::getKey(){
    return employee_ptr->getId();
}

void EmployeeHashtableVal::setNullListNode() {
    list_node = nullptr;
}


void EmployeeHashtableVal::setListNode(DoublyLinkedListNode<Employee*>* new_list_node) {
    list_node = new_list_node;
}

void EmployeeHashtableVal::setTreeNode(Node<EmployeeKey, Employee*, EmployeeRank>* new_tree_node) {
    tree_node = new_tree_node;
}

void EmployeeHashtableVal::setEmployeePtr(Employee* other_employee_ptr){
    employee_ptr = other_employee_ptr;
}

bool EmployeeHashtableVal::operator==(EmployeeHashtableVal* other_node) {
    if (other_node == nullptr) {
        return (*this == nullptr);
    }
    if (other_node->employee_ptr == nullptr) {
        return (this->employee_ptr == nullptr);
    }
    return (employee_ptr->getId() == other_node->employee_ptr->getId());
}

EmployeeHashtableVal& EmployeeHashtableVal::operator=(EmployeeHashtableVal *other_node) {
    if (other_node == nullptr) {
        employee_ptr = nullptr;
        tree_node = nullptr;
        list_node = nullptr;
    }
    else {
        employee_ptr = other_node->employee_ptr;
        tree_node = other_node->tree_node;
        list_node = other_node->list_node;
    }
    return *this;
}


