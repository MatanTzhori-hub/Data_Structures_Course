#ifndef UNION_FIND_H
#define UNION_FIND_H


#include <cstdlib>
#include <stdexcept>
#include "up_tree_node.h"
#include "rank_tree.h"


/*
 * Union DS expects index values from 0 to size (not including size).
 * */

template <typename data_t>
class Union{
    int size;
    UpTreeNode<data_t>** array_base;
public:

    Union() = default;
    explicit Union(int new_size);
    ~Union();
    void killAllCompanies();
    ReturnValue makeSet(int index, data_t data);
    ReturnValue findNodeRepByID(int index, UpTreeNode<data_t>** node);
    ReturnValue getNodeByID(int index, UpTreeNode<data_t>** node);
    ReturnValue findDataPtrByIndex(int index, data_t* data_ptr);
    ReturnValue unify(int index1, int index2, double factor);
    double calcTrueValue(int index);
    ReturnValue bumpGradeToEmployees(int lowerSalary, int higherSalary, long long BumpGrade);
};

/* union gets num of elements in structure, creates new array with correct size.
 union doesn't create new groups, only array with correct amount of nodes, user needs to create actual new groups
 and insert each group to the correct node in array.*/
template<typename data_t>
Union<data_t>::Union(int new_size) : size(new_size) {
    // create empty array of node pointers
    array_base = new UpTreeNode<data_t>*[size];
    if(!array_base){
        throw std::bad_alloc();
    }
}


template<typename data_t>
Union<data_t>::~Union(){
    for (int i = 0; i < size; i++){
        delete array_base[i];
    }

    delete[] array_base;
}


template <typename data_t>
void Union<data_t>::killAllCompanies(){
    for (int i = 0; i < size; i++){
        delete array_base[i]->getData();
    }
}


template <typename data_t>
ReturnValue Union<data_t>::makeSet(int index, data_t data) {
    if (index < 0 || index >= size){
        return MY_INVALID_INPUT;
    }

    UpTreeNode<data_t>* node_ptr = new UpTreeNode<data_t>(data);
    if (node_ptr == nullptr){
        return MY_ALLOCATION_ERROR;
    }

    *(array_base + index) = node_ptr;
    return MY_SUCCESS;
}

template <typename data_t>
ReturnValue Union<data_t>::findNodeRepByID(int index, UpTreeNode<data_t>** node){
    // check input
    if (index < 0 || index >= size){
        return MY_INVALID_INPUT;
    }

    // start at array base (array starts with index=0)
    UpTreeNode<data_t>* curr_node = array_base[index];

    // find rep node for the upTree
    double bump_on_way = 0;
    while (!(curr_node->isRoot())){
        bump_on_way += curr_node->value_bump;
        curr_node = curr_node->getFather();
    }

    if (curr_node == nullptr){
        return ELEMENT_DOES_NOT_EXIST;
    }

    ReturnValue res;
    // shrink routes - update all nodes along route to point to root of upTree
    UpTreeNode<data_t>* root = curr_node; // at this point, curr_node is the root
    UpTreeNode<data_t>* father;
    curr_node = *(array_base + index); // reset cur_node to the original node of index

    if(index == 17 || index == 13 || index == 2)
    {
        printf("");
    }

    while (!(curr_node->isRoot())){
        father = curr_node->getFather();
        if (!father->isRoot()){
            res = curr_node->setFather(root);
            bump_on_way -= curr_node->value_bump;
            curr_node->value_bump += bump_on_way;
            if ( res != MY_SUCCESS){
                return res;
            }
        }
        curr_node = father;
    }

    // root is the node rep of the upTree, so that's the node we return
    *node = root;
    return MY_SUCCESS;
}

template <typename data_t>
ReturnValue Union<data_t>::getNodeByID(int index, UpTreeNode<data_t>** node){
    // check input
    if (index < 0 || index >= size){
        return MY_INVALID_INPUT;
    }

    // start at array base (array starts with index=0)
    *node = array_base[index];
    return MY_SUCCESS;
}

template <typename data_t>
ReturnValue Union<data_t>::findDataPtrByIndex(int index, data_t* data_ptr){
    UpTreeNode<data_t>* node;
    ReturnValue res = findNodeRepByID(index, &node);
    if (res != MY_SUCCESS){
        return res;
    }

    *data_ptr = node->getData();
    return MY_SUCCESS;
}

template <typename data_t>
ReturnValue Union<data_t>::unify(int index1, int index2, double factor){
    // check input
    if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size){
        return MY_INVALID_INPUT;
    }

    // check if same element(no need to merge anything)
    if (index1 == index2){
        return MY_SUCCESS;
    }

    // find rep node of each element (elements are saved in reversed tree)
    UpTreeNode<data_t>* buyer_node;
    UpTreeNode<data_t>* bought_node;
    ReturnValue res = findNodeRepByID(index1, &buyer_node);
    if (res == MY_INVALID_INPUT || res == ELEMENT_DOES_NOT_EXIST){
        return res;
    }
    res = findNodeRepByID(index2, &bought_node);
    if (res == MY_INVALID_INPUT || res == ELEMENT_DOES_NOT_EXIST){
        return res;
    }

    // check if both elements have the same rep node (if so, no need to merge)
    if (buyer_node == bought_node){
        return MY_INVALID_INPUT;
    }

    if (*buyer_node < *bought_node) {
        buyer_node->data->setValue(buyer_node->data->getValue() + buyer_node->getValueBump());
        bought_node->data->setValue(bought_node->data->getValue() + bought_node->getValueBump());
        
        // Mark the names for later used after switch
        UpTreeNode<data_t>* new_buyer_node = bought_node;
        UpTreeNode<data_t>* new_bought_node = buyer_node;

        // Switch location of buyer and bought
        data_t temp = buyer_node->data;
        buyer_node->data = bought_node->data;
        bought_node->data = temp;

        // Switch pointers in the array
        UpTreeNode<data_t>* temp_ptr = array_base[buyer_node->data->getId()];
        array_base[buyer_node->data->getId()] = array_base[bought_node->data->getId()];
        array_base[bought_node->data->getId()] = temp_ptr;

        new_buyer_node->data->setValue(new_buyer_node->data->getValue() + factor * new_bought_node->data->getValue());
        new_bought_node->value_bump += factor * new_bought_node->data->getValue();
        new_bought_node->data->setValue(new_bought_node->data->getValue() - new_bought_node->value_bump);
        new_buyer_node->data->setValue(new_buyer_node->data->getValue() - new_buyer_node->value_bump);
        new_bought_node->value_bump -= new_buyer_node->value_bump;

        // this is for the uniteUpTreeNodes out of the if
        buyer_node = new_buyer_node;
        bought_node = new_bought_node;
    }
    else{
        buyer_node->value_bump += factor * (bought_node->data->getValue() + bought_node->value_bump);
        bought_node->value_bump -= buyer_node->value_bump;
    }

    res = buyer_node->UniteUpTreeNodes(bought_node);

    return res;
}


template <typename data_t>
double Union<data_t>::calcTrueValue(int index){
    if (index < 0 || index >= size){
        return -1;
    }

    UpTreeNode<data_t>* node;
    ReturnValue res = getNodeByID(index, &node);
    if(res != MY_SUCCESS){
        return res;
    }

    double total_value = node->getData()->getValue();

    while(node->getFather()){
        total_value += node->getValueBump();
        node = node->getFather();
    }
    total_value += node->getValueBump();

    return total_value;
}


template <typename data_t>
ReturnValue Union<data_t>::bumpGradeToEmployees(int lowerSalary, int higherSalary, long long BumpGrade){
    for(int i =0; i < size; i++){
        UpTreeNode<data_t>** node;
        getNodeByID(i, node);
        
        (*node)->data->bumpGradeInRange(lowerSalary, higherSalary, BumpGrade);
    }

    return MY_SUCCESS;
}


#endif //UNION_FIND_H
