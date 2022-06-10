#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "doubly_linked_list_node.h"
#include "rank_tree.h"

template <typename data_t>
class DoublyLinkedList {
    DoublyLinkedListNode<data_t>* head;
    DoublyLinkedListNode<data_t>* tail;
    int size;
    int grade_bump;
    long long grade_sum;
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0), grade_bump(0), grade_sum(0) {}
    ~DoublyLinkedList();
    ReturnValue insert(data_t new_data);
    ReturnValue remove(DoublyLinkedListNode<data_t>* node_to_remove);
    int getSize() const{ return size; }
    ReturnValue merge_to_me(DoublyLinkedList<data_t> &other_list);
    DoublyLinkedListNode<data_t>* getHead() { return head; }
    DoublyLinkedListNode<data_t>* getTail() { return tail; }
    long long getGradeSum();
    void bumpAllGrade(int bump_amount);
};

template <typename data_t>
ReturnValue DoublyLinkedList<data_t>::insert(data_t new_data){
    DoublyLinkedListNode<data_t>* new_node = new DoublyLinkedListNode<data_t>(new_data);
    if(!new_node){
        return MY_ALLOCATION_ERROR;
    }
    new_node->next = head;
    if(head != nullptr){
        head->prev = new_node;
    }
    else{
        tail = new_node;
    }
    head = new_node;
    size++;

    // adding original grade to grade_sum, and updating the grade of the employee to
    // hold original grade minus the current grade_bump
    new_node->getData()->setGrade(new_node->getData()->getGrade() - grade_bump);
    grade_sum += new_node->getData()->getGrade();

    return MY_SUCCESS;
}

template <typename data_t>
DoublyLinkedList<data_t>::~DoublyLinkedList(){
    DoublyLinkedListNode<data_t>* curr_node = head;
    DoublyLinkedListNode<data_t>* temp = curr_node;
    
    while(curr_node != nullptr){
        temp = curr_node;
        curr_node = curr_node->next;
        delete temp;
    }
}

template <typename data_t>
ReturnValue DoublyLinkedList<data_t>::remove(DoublyLinkedListNode<data_t>* node_to_remove){
    //we assume the node does in face exists in the list

    if(size == 0){
        return ELEMENT_DOES_NOT_EXIST;
    }
    if(size == 1){
        head = nullptr;
        tail = nullptr;
    }
    else{
        if(head == node_to_remove){
            head = node_to_remove->next;
            node_to_remove->next = nullptr;
            head->prev = nullptr;
        }
        else if(tail == node_to_remove) {
            tail = node_to_remove->prev;
            node_to_remove->prev = nullptr;
            tail->next = nullptr;
        }
        else{
            node_to_remove->prev->next = node_to_remove->next;
            node_to_remove->next->prev = node_to_remove->prev;

            node_to_remove->next = nullptr;
            node_to_remove->prev = nullptr;
        }
    }

    // todo : check with adi what is the correct order here:
    grade_sum -= node_to_remove->getData()->getGrade();
    node_to_remove->getData()->setGrade(node_to_remove->getData()->getGrade() + grade_bump);
    //

    delete node_to_remove;
    size--;

    return MY_SUCCESS;
}

template <typename data_t>
ReturnValue DoublyLinkedList<data_t>::merge_to_me(DoublyLinkedList<data_t> &other_list){
    if(other_list.size == 0){
        return MY_SUCCESS;
    }

    if(size == 0){
        head = other_list.head;
        tail = other_list.tail;
        size = other_list.size;
        grade_bump = other_list.grade_bump;
        grade_sum = other_list.grade_sum;

        other_list.head = nullptr;
        other_list.tail = nullptr;
        other_list.size = 0;
        other_list.grade_bump = 0;
        other_list.grade_sum = 0;
        return MY_SUCCESS;
    }

    int total_sum = 0;
    DoublyLinkedListNode<data_t>* temp_node = head;
    while(temp_node){
        temp_node->getData()->setGrade(temp_node->getData()->getGrade() + grade_bump);
        total_sum += temp_node->getData()->getGrade();
        temp_node = temp_node->getNext();
    }
    grade_bump = 0;

    temp_node = other_list.getHead();
    while(temp_node){
        temp_node->getData()->setGrade(temp_node->getData()->getGrade() + other_list.grade_bump);
        total_sum += temp_node->getData()->getGrade();
        temp_node = temp_node->getNext();
    }
    other_list.grade_bump = 0;


    tail->next = other_list.head;
    other_list.head->prev = tail;
    tail = other_list.tail;
    size += other_list.size;
    other_list.head = nullptr;
    other_list.tail = nullptr;
    other_list.size = 0;
    other_list.grade_sum = 0;
    grade_sum = total_sum;
    return MY_SUCCESS;
}

template <typename data_t>
long long DoublyLinkedList<data_t>::getGradeSum(){
    return grade_sum + (size * grade_bump);
}


template <typename data_t>
void DoublyLinkedList<data_t>::bumpAllGrade(int bump_amount){
    grade_bump += bump_amount;
}

#endif //DOUBLY_LINKED_LIST_H
