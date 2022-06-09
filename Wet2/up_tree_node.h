#ifndef WET2_UP_TREE_NODE_H
#define WET2_UP_TREE_NODE_H

#include "rank_tree.h"


template <typename data_t>
class Union;


template<typename data_t>
class UpTreeNode {
    data_t data;
    UpTreeNode<data_t>* father;
    int size;
    double value_bump;

public:
    UpTreeNode() : data(nullptr), father(nullptr), size(1), value_bump(0) {}
    explicit UpTreeNode(data_t new_data) : data(new_data), father(nullptr), size(1), value_bump(0) {}
    ~UpTreeNode() = default;
    UpTreeNode* getFather() { return father; }
    data_t getData() { return data; }
    int getSize() { return size; }
    bool isRoot() { return father == nullptr; }
    ReturnValue setFather(UpTreeNode<data_t>* new_father);
    void setData(data_t new_data) { data = new_data; }
    void setValueBump(double new_bump) { value_bump = new_bump; }
    double getValueBump() { return value_bump; }
    ReturnValue UniteUpTreeNodes(UpTreeNode<data_t>* other_node, double factor);
    bool operator<(UpTreeNode<data_t> node2);

    friend class Union<data_t>;
};

template<typename data_t>
ReturnValue UpTreeNode<data_t>::setFather(UpTreeNode<data_t>* new_father){
    if (new_father == this) {
        return MY_FAILURE;
    }

    father = new_father;
    return MY_SUCCESS;
}

template<typename data_t>
ReturnValue UpTreeNode<data_t>::UniteUpTreeNodes(UpTreeNode<data_t>* other_node, double factor){
    // check input
    if (other_node == nullptr){
        return MY_INVALID_INPUT;
    }

    // check both are roots, we only merge nodes that are reps of their upTree
    if (!(this->father == nullptr) || !(other_node->father == nullptr)){
        return MY_FAILURE;
    }

    // merge data (actual object) of other node in to this node
    *data += *(other_node->data);

    // update size of this (UpTreeNode)
    this->size += other_node->size;
    
    // We might not want to switch, because then when we try to calculate the true value
    // of 1 company, we might get the value of the 1 we switched with.
    // data_t temp_ptr = other_node->data;
    // other_node->data = data;
    // data = temp_ptr;

    // double temp_bump = other_node->value_bump;
    // other_node->value_bump = value_bump;
    // value_bump = temp_bump;


    // update other_node father to this (other node is no longer root)
    return other_node->setFather(this);
}

template<typename data_t>
bool UpTreeNode<data_t>::operator<(UpTreeNode<data_t> node2){
    if(size < node2.size){
        return true;
    }
    return false;
}


#endif //WET2_UP_TREE_NODE_H
