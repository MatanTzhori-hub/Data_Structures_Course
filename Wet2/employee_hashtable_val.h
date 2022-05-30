#ifndef EMPLOYEE_HASHTABLE_VAL_H
#define EMPLOYEE_HASHTABLE_VAL_H

#include "employee.h"
#include "employee_rank.h"
#include "node.h"
#include "doubly_linked_list_node.h"

class EmployeeHashtableVal {
    Employee* employee_ptr;
    Node<EmployeeKey, Employee*, EmployeeRank>* tree_node;
    DoublyLinkedListNode<Employee*>* list_node;

public:
    EmployeeHashtableVal(Employee* employee_ptr, Node<EmployeeKey, Employee*, EmployeeRank>* tree_node, DoublyLinkedListNode<Employee*>* list_node) :
                                                        employee_ptr(employee_ptr), 
                                                        tree_node(tree_node), 
                                                        list_node(list_node) {}
    explicit EmployeeHashtableVal(Employee* employee_ptr) : employee_ptr(employee_ptr),
                                                            tree_node(nullptr), 
                                                            list_node(nullptr) {}
    EmployeeHashtableVal() : employee_ptr(nullptr),
                             tree_node(nullptr), 
                             list_node(nullptr){}
    ~EmployeeHashtableVal();
    Employee* getEmployeePtr() { return employee_ptr; }

    void initializeVal();
    int getKey() { return employee_ptr->getId(); }
    DoublyLinkedListNode<Employee*>* getListNode() { return list_node; }
    Node<EmployeeKey, Employee*, EmployeeRank>* getTreeNode() { return tree_node; }
    void setNullListNode();
    void setListNode(DoublyLinkedListNode<Employee*>* new_list_node);
    void setTreeNode(Node<EmployeeKey, Employee*, EmployeeRank>* new_tree_node);
    void setEmployeePtr(Employee* employee_ptr);

    bool operator==(EmployeeHashtableVal* other_node);
    EmployeeHashtableVal& operator=(EmployeeHashtableVal* other_node);

    EmployeeHashtableVal* getEmployeeHashtableVal() { return this; }

};


#endif //EMPLOYEE_HASHTABLE_VAL_H

