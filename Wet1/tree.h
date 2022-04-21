#ifndef TREE_H
#define TREE_H

#include "iterator.h"
#include <cmath>
#include <iostream>

typedef enum {MY_ALLOCATION_ERROR, MY_INVALID_INPUT, MY_FAILURE, MY_SUCCESS, ELEMENT_EXISTS,
	ELEMENT_DOES_NOT_EXIST, NO_ELEMENT_INSERT_LEFT, NO_ELEMENT_INSERT_RIGHT, NO_ROOT} ReturnValue;


using std::ostream;

template<typename key_t, typename data_t>
class Tree {
	Node<key_t, data_t>* root;
	int size;

	//tree rolls
	ReturnValue fixTree(Iterator<key_t, data_t>& iter);
	ReturnValue RollTree(Node<key_t, data_t>& node);
	ReturnValue LLRoll(Node<key_t, data_t>& node);
	ReturnValue RRRoll(Node<key_t, data_t>& node);
	ReturnValue LRRoll(Node<key_t, data_t>& node);
	ReturnValue RLRoll(Node<key_t, data_t>& node);

	//Node Removal Helper Functions
	ReturnValue removeRoot(Iterator<key_t, data_t>& iter);
	ReturnValue removeNonRoot(Iterator<key_t, data_t>& iter);

    //Node Swaps
	ReturnValue swapNodes(Node<key_t, data_t>* node1, Node<key_t, data_t>* node2);
	void swapRoot(Node<key_t, data_t>* received_root, Node<key_t, data_t>* node);
	void swapNonRoot(Node<key_t, data_t>* node1, Node<key_t, data_t>* node2);

    //Tree Merging Helper Function and Calculations
	int findRequiredEmptyTreeHeight(int n);
	int findNumOfNodesToDelete(int size);
	Node<key_t, data_t>* createEmptyTree(int* height);
	bool removeExtraTreeNodes(Node<key_t, data_t>* node, int* num_of_nodes_to_delete);

	//Array casting
	void putTreeToArray(Node<key_t, data_t>* node, Node<key_t, data_t>* arr[], int* i);
    void putArrayIntoTree(Node<key_t, data_t>** tree_node, Node<key_t, data_t>*** array, int* i); 
	
	//Array merge
	void mergeArrays(Node<key_t, data_t>* arr1[], Node<key_t, data_t>* arr2[],
					   Node<key_t, data_t>* merged_arr[], int arr1_size, int arr2_size);

	// // Work in progress
	// class TreeIterator{
	// 		Node<key_t, data_t>* curr_node;
	// 		Node<key_t, data_t>* last_node;
	
	// 	public:
	// 	explicit TreeIterator(Node<key_t, data_t>* curr_node) noexcept: next_node(curr_node), last_node(nullptr) {}
	// 	~TreeIterator(){}
	// 	TreeIterator<key_t, data_t>& next();
	// 	data_t getData() const {return curr_node->getData(); }
	// 	key_t getKey() const {return curr_node->getKey(); }
	// 	bool operator==()
	// };
	// //...


public:
	Tree() : size(0) {root = nullptr; }
	~Tree();

	//Add or Remove elements
	void deleteTreeNodes(Iterator<key_t, data_t>& iter);
	ReturnValue insert(key_t key, data_t element);
	ReturnValue removeElement(key_t key);

	ReturnValue findElement(key_t key, Iterator<key_t, data_t>& iter);
	int getSize() const;
	Iterator<key_t, data_t> begin();
	Iterator<key_t, data_t> begin() const;
	Iterator<key_t, data_t> createIterByNode(Node<key_t, data_t>& node);
	Iterator<key_t, data_t> createIterByNode(Node<key_t, data_t>& node) const;
	void getRootIndexInOrder(Node<key_t, data_t>* node, int* index_ptr);
	void getRightMostNode(Node<key_t, data_t>** node_ptr);
	Iterator<key_t, data_t> Tree<key_t, data_t>::getRightMost();

	// Print functions
	void printTree();
	void printInOrder(Node<key_t, data_t>* node);
	
	// Merge other tree to this tree
	Tree<key_t, data_t>* mergeToMe(Tree<key_t, data_t>& tree2);
};

template<typename key_t, typename data_t>
Tree<key_t, data_t>::~Tree<key_t, data_t>() {
	deleteTreeNodes(this->begin());
	delete root;
}


template<typename key_t, typename data_t>
void Tree<key_t, data_t>::deleteTreeNodes(Iterator<key_t, data_t>& iter) {
	if(iter.getPtr() == nullptr){
		return;
	}
	deleteTreeNodes(iter.getPtr()->left);
	deleteTreeNodes(iter.getPtr()->right);
	if(iter.getPtr() != root){
		iter.getPtr()->isALeftSon() ? iter.getPtr()->father->left = nullptr : iter.getPtr()->father->right = nullptr;
		iter.getPtr()->father = nullptr;
		delete iter.getPtr();
	}
}


template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::findElement(key_t key, Iterator<key_t, data_t>& iter) {
	if(iter.ptr == nullptr) {
		return NO_ROOT;
	}
	Iterator<key_t, data_t> temp_iter = iter;
	while(temp_iter.ptr){
		if(iter.ptr->key == key){
			return ELEMENT_EXISTS;
		}
		else if(iter.ptr->key > key){
			temp_iter.goLeft();
			if(temp_iter.ptr){
				iter.goLeft();
			}
		}
		else{
			temp_iter.goRight();
			if(temp_iter.ptr){
				iter.goRight();
			}
		}
	}
	if(iter.ptr->key > key){
		return NO_ELEMENT_INSERT_LEFT;
	}
	return NO_ELEMENT_INSERT_RIGHT;
}


template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::insert(key_t key, data_t element) {
	auto* node_to_insert = new Node<key_t, data_t>(key,element);	//this here is roy and adi fix and is important
	if(!node_to_insert){
		return MY_ALLOCATION_ERROR;
	}

	if(!root){
		size++;
		root = node_to_insert;
		return MY_SUCCESS;
	}
	Iterator<key_t, data_t> iter = begin();
	/**if(!iter){
		return MY_ALLOCATION_ERROR;
	}*/
	//check height when son is nullptr
	ReturnValue result = findElement(key, iter);
	switch (result) {
		case MY_ALLOCATION_ERROR :
			return MY_ALLOCATION_ERROR;
		case NO_ROOT :
			size++;
			root = node_to_insert;
			return MY_SUCCESS;
		case ELEMENT_EXISTS :
			return MY_FAILURE;
		case NO_ELEMENT_INSERT_LEFT :
			iter.ptr->left = node_to_insert;
			node_to_insert->father = iter.ptr;
			break;
		case NO_ELEMENT_INSERT_RIGHT :
			iter.ptr->right = node_to_insert;
			node_to_insert->father = iter.ptr;
		default :
			break;
	}
	size++;
	return fixTree(iter);
}


template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::fixTree(Iterator<key_t, data_t>& iter) {
	int old_height = 0;
	while(iter.ptr != nullptr){
		old_height = iter.ptr->height;
		iter.ptr->updateHeight();
		iter.ptr->updateBF();
		if(abs(iter.ptr->balance_factor) == 2){
			if(RollTree(*iter.ptr) == MY_ALLOCATION_ERROR) {
				return MY_ALLOCATION_ERROR;
			}
			iter.goFather();
		}
        if(iter.ptr == nullptr){
            break;
        }
        if(old_height == iter.ptr->height){
			break;
		}
		iter.goFather();
	}
	return MY_SUCCESS;
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::RollTree(Node<key_t, data_t>& node) {
	if(node.balance_factor == 2){
		if(node.left->getBalanceFactor() >= 0){
			return LLRoll(node);
		}
		else if(node.left->getBalanceFactor() == -1)
			return LRRoll(node);
	}
	if(node.balance_factor == -2){
		if(node.right->getBalanceFactor() <= 0){
			return RRRoll(node);
		}
		else if(node.right->getBalanceFactor() == 1){
			return RLRoll(node);
		}
	}
	return MY_SUCCESS;
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::LLRoll(Node<key_t, data_t>& node) {
	Node<key_t, data_t>* saved_left_ptr = node.left;
	if(!saved_left_ptr){
		return MY_ALLOCATION_ERROR;
	}
	saved_left_ptr->father = node.father;
	if(node.isARightSon()){
		saved_left_ptr->father->right = saved_left_ptr;
	}
	else if(node.isALeftSon()){
		saved_left_ptr->father->left = saved_left_ptr;
	}
	node.left = saved_left_ptr->right;
	if(saved_left_ptr->right){
		saved_left_ptr->right->father = &node;
	}
	saved_left_ptr->right = &node;
	node.father = saved_left_ptr;
	if(saved_left_ptr->father == nullptr){
		root = saved_left_ptr;
	}
	node.updateHeight();
	node.updateBF();
	saved_left_ptr->updateHeight();
	saved_left_ptr->updateBF();
	return MY_SUCCESS;
}


template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::RRRoll(Node<key_t, data_t> &node) {
	Node<key_t, data_t>* saved_right_ptr = node.right;
	if(!saved_right_ptr){
		return MY_ALLOCATION_ERROR;
	}
	saved_right_ptr->father = node.father;
	if(node.isARightSon()){
		saved_right_ptr->father->right = saved_right_ptr;
	}
	else if(node.isALeftSon()){
		saved_right_ptr->father->left = saved_right_ptr;
	}
	node.right = saved_right_ptr->left;
	if(saved_right_ptr->left){
		saved_right_ptr->left->father = &node;
	}
	saved_right_ptr->left = &node;
	node.father = saved_right_ptr;
	if(saved_right_ptr->father == nullptr){
		root = saved_right_ptr;
	}
	node.updateHeight();
	node.updateBF();
	saved_right_ptr->updateHeight();
	saved_right_ptr->updateBF();
	return MY_SUCCESS;
}


template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::LRRoll(Node<key_t, data_t> &node) {
	if(RRRoll(*(node.left)) == MY_SUCCESS){
		if (LLRoll(node) == MY_SUCCESS){
			return MY_SUCCESS;
		}
	}
	return MY_ALLOCATION_ERROR;
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::RLRoll(Node<key_t, data_t> &node) {
	if(LLRoll(*(node.right)) == MY_SUCCESS){
		if(RRRoll(node) == MY_SUCCESS){
			return MY_SUCCESS;
		}
	}
	return MY_ALLOCATION_ERROR;
}

template<typename key_t, typename data_t>
Iterator<key_t, data_t> Tree<key_t, data_t>::begin() {
	return Iterator<key_t, data_t>(root);
}

template<typename key_t, typename data_t>
Iterator<key_t, data_t> Tree<key_t,data_t>::begin() const {
	return Iterator<key_t, data_t>(root);
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::removeElement(key_t key) {
	Iterator<key_t, data_t> iter = begin();

	ReturnValue result = findElement(key, iter);
	switch (result) {
		case MY_ALLOCATION_ERROR :
			return MY_ALLOCATION_ERROR;
		case ELEMENT_EXISTS:
			break;
		default:
			return MY_FAILURE;
	}
	if(iter.ptr == root){
		return removeRoot(iter);
	}
	else{
		return removeNonRoot(iter);
	}
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::removeRoot(Iterator<key_t, data_t> &iter) {
	Node<key_t, data_t>* to_delete = iter.ptr;
	if(!to_delete){
		return MY_ALLOCATION_ERROR;
	}
	if(iter.ptr->isLeaf()){
		root = nullptr;
		iter.ptr = nullptr;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else if(iter.ptr->onlyHaveRightSon()) {
		iter.ptr->right->father = nullptr;
		root = iter.ptr->right;
		iter.ptr = nullptr;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else if(iter.ptr->onlyHaveLeftSon()){
		iter.ptr->left->father = nullptr;
		root = iter.ptr->left;
		iter.ptr = nullptr;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else{
		iter.goRight();
		while(iter.ptr->left != nullptr){
			iter.goLeft();
		}
		if(swapNodes(to_delete, iter.ptr) == MY_ALLOCATION_ERROR){
			return MY_ALLOCATION_ERROR;
		}
        Iterator<key_t, data_t> new_iter = createIterByNode(*to_delete);
		return removeNonRoot(new_iter);
	}
}

template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::removeNonRoot(Iterator<key_t, data_t> &iter) {
	Node<key_t, data_t>* to_delete = iter.ptr;
	if(!to_delete){
		return MY_ALLOCATION_ERROR;
	}
	if(iter.ptr->haveTwoSons()){
		iter.goRight();
		while(iter.ptr->left){
			iter.goLeft();
		}
		if(swapNodes(to_delete, iter.ptr) == MY_ALLOCATION_ERROR){
			return MY_ALLOCATION_ERROR;
		}
	}

	Iterator<key_t, data_t> new_iter = createIterByNode(*to_delete);

	if(new_iter.ptr->isLeaf()){
		if(new_iter.ptr->isALeftSon()){
			new_iter.ptr->father->left = nullptr;
		}
		else if(new_iter.ptr->isARightSon()){
			new_iter.ptr->father->right = nullptr;
		}
		new_iter.goFather();
		delete to_delete;
		size--;
		return fixTree(new_iter);
	}
	if(new_iter.ptr->onlyHaveLeftSon()){
		if(new_iter.ptr->isALeftSon()){
			new_iter.ptr->father->left = new_iter.ptr->left;
			new_iter.ptr->left->father = new_iter.ptr->father;
			new_iter.goFather();
            delete to_delete;
			size--;
			return fixTree(new_iter);
		} else if(new_iter.ptr->isARightSon()){
			new_iter.ptr->father->right = new_iter.ptr->left;
			new_iter.ptr->left->father = new_iter.ptr->father;
			new_iter.goFather();
            delete to_delete;
			size--;
			return fixTree(new_iter);
		}
	}
	else if(new_iter.ptr->onlyHaveRightSon()){
		if(new_iter.ptr->isALeftSon()){
			new_iter.ptr->father->left = new_iter.ptr->right;
			new_iter.ptr->right->father = new_iter.ptr->father;
			new_iter.goFather();
            delete to_delete;
			size--;
			return fixTree(new_iter);
		} else if(new_iter.ptr->isARightSon()){
			new_iter.ptr->father->right = new_iter.ptr->right;
			new_iter.ptr->right->father = new_iter.ptr->father;
			new_iter.goFather();
            delete to_delete;
			size--;
			return fixTree(new_iter);
		}
	}
	return MY_SUCCESS;
}


/*
 * Assumes the node to be deleted is node1, and the other one is node2
 */
template<typename key_t, typename data_t>
ReturnValue Tree<key_t, data_t>::swapNodes(Node<key_t, data_t>* node1, Node<key_t, data_t>* node2) {
	if(root == node1){
		swapRoot(node1, node2);
	}
	else{
		swapNonRoot(node1, node2);
	}
	return MY_SUCCESS;
}

template<typename key_t, typename data_t>
Iterator<key_t, data_t> Tree<key_t, data_t>::createIterByNode(Node<key_t, data_t> &node) {
	return Iterator<key_t, data_t>(&node);
}


template<typename key_t, typename data_t>
Iterator<key_t, data_t> Tree<key_t, data_t>::createIterByNode(Node<key_t, data_t> &node) const {
	return Iterator<key_t, data_t>(&node);
}


using std::cout;

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::printTree() {
	printInOrder(root);
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::printInOrder(Node<key_t, data_t>* node) {
	if(node == nullptr){
		return;
	}
	printInOrder(node->left);
	node->printNode();
	printInOrder(node->right);

}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::getRightMostNode(Node<key_t, data_t>** node_ptr){
	Iterator<key_t, data_t> iter = begin();
	if(iter.ptr == nullptr) {
		node_ptr = nullptr;
	}
	Iterator<key_t, data_t> temp_iter = begin();
	if(temp_iter.ptr == nullptr){
		node_ptr = nullptr;
	}
	while(temp_iter.ptr){
		if(iter.ptr->getRight() == nullptr){
			*node_ptr = iter.getPtr();
			return;
		}
		iter.goRight();
		temp_iter.goRight();
	}
	*node_ptr = iter.getPtr();
}

template<typename key_t, typename data_t>
Iterator<key_t, data_t> Tree<key_t, data_t>::getRightMost(){
	Iterator<key_t, data_t> iter = begin();
	if(iter.getPtr() == nullptr) {
		return iter;
	}
	while(!iter.checkNullRight()){
		iter.goRight();
	}

	return iter;
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::swapRoot(Node<key_t, data_t> *received_root, Node<key_t, data_t> *node) {
	Node<key_t, data_t>* orig_father_of_node = node->father;
	Node<key_t, data_t>* orig_right_son_of_node = node->right;
	if(received_root->right != node){
		node->right = received_root->right;
		if(received_root->right){
			received_root->right->father = node;
		}
		node->left = received_root->left;
		if(received_root->left){
			received_root->left->father = node;
		}
		node->father = nullptr;
		root = node;
		received_root->father = orig_father_of_node;
		orig_father_of_node->left = received_root;
		received_root->right = orig_right_son_of_node;
		received_root->left = nullptr;
		if(orig_right_son_of_node != nullptr){
			orig_right_son_of_node->father = received_root;
		}
	}
	else if(received_root->right == node){
		node->right = received_root;
		node->left = received_root->left;
		if(received_root->left){
			received_root->left->father = node;
		}
		node->father = nullptr;
		root = node;
		received_root->father = node;
		received_root->left = nullptr;
		received_root->right = orig_right_son_of_node;
		if(orig_right_son_of_node != nullptr){
			orig_right_son_of_node->father = received_root;
		}
	}
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::swapNonRoot(Node<key_t, data_t> *node1, Node<key_t, data_t> *node2) {
	Node<key_t, data_t>* orig_father_of_node2 = node2->father;
	Node<key_t, data_t>* orig_right_son_of_node2 = node2->right;
	if(node1->right != node2){
		node2->right = node1->right;
		if(node1->right){
			node1->right->father = node2;
		}
		node2->left = node1->left;
		if(node1->left){
			node1->left->father = node2;
		}
		node2->father = node1->father;
		if(node1->isALeftSon()){
			node1->father->left = node2;
		}
		else if(node1->isARightSon()){
			node1->father->right = node2;
		}
		node1->father = orig_father_of_node2;
		orig_father_of_node2->left = node1;
		node1->left = nullptr;
		node1->right = orig_right_son_of_node2;
		if(orig_right_son_of_node2 != nullptr){
			orig_right_son_of_node2->father = node1;
		}
	}
	else if(node1->right == node2){
		node2->right = node1;
		node2->left = node1->left;
		if(node1->left){
			node1->left->father = node2;
		}
		node2->father = node1->father;
		if(node1->isALeftSon()){
			node1->father->left = node2;
		}
		else if(node1->isARightSon()){
			node1->father->right = node2;
		}
		node1->father = node2;
		node1->left = nullptr;
		node1->right = orig_right_son_of_node2;
		if(orig_right_son_of_node2 != nullptr){
			orig_right_son_of_node2->father = node1;
		}
	}
}

template<typename key_t, typename data_t>
Tree<key_t, data_t>* Tree<key_t, data_t>::mergeToMe(Tree<key_t, data_t>& other_tree) {
	if (other_tree.getSize() == 0){
        return;
    }

	/*
	 * create arrays for each tree, and merged tree
	 * */
	Node<key_t, data_t>** arr1 = new Node<key_t, data_t>*[sizeof(Node<key_t, data_t>*) * getSize()];
	Node<key_t, data_t>** arr2 = new Node<key_t, data_t>*[sizeof(Node<key_t, data_t>*) * other_tree.getSize()];
	Node<key_t, data_t>** merged_arr = new Node<key_t, data_t>*[sizeof(Node<key_t, data_t>*) * (getSize()+other_tree.getSize())];

	/*
	 * insert all nodes of each tree to it's array
	 * */
	int i=0 , j = 0;
	this->putTreeToArray(root, arr1, &i);
	other_tree.putTreeToArray(other_tree.root, arr2, &j);

	/*
	 * merge the 2 arrays to 1 array
	 * */
	mergeArrays(arr1, arr2, merged_arr, size, other_tree.getSize());

	/*
	 * create new tree with new size
	 * */
	int merged_size = size + other_tree.getSize();
	int req_tree_height = findRequiredEmptyTreeHeight(merged_size);
	Node<key_t, data_t>* new_root = createEmptyTree(&req_tree_height);
	int num_of_nodes_to_delete_int = findNumOfNodesToDelete(merged_size);
	removeExtraTreeNodes(new_root, &num_of_nodes_to_delete_int);

	int m=0;
	Node<key_t, data_t>* old_root = this.root;
	this.root = new_root;
    putArrayIntoTree(&new_root, &merged_arr, &m);
	this.size = merged_size;

	Tree<key_t, data_t> temp_tree = Tree<key_t, data_t>();
	temp_tree.root = old_root;
	temp_tree.~Tree<key_t, data_t>();

	delete[] arr1;
    delete[] arr2;
    delete[] merged_arr;

	return this;
}


template<typename key_t, typename data_t>
void Tree<key_t, data_t>::putArrayIntoTree(Node<key_t, data_t>** tree_node, Node<key_t, data_t>*** array, int* i) {
	if (!(*tree_node)) {
		return;
	}

    if((*tree_node)->left){
        putArrayIntoTree(&(*tree_node)->left, array, i);
    }

    (*tree_node)->updateData((*(*array)[*i]).getData());
    (*tree_node)->updateKey((*(*array)[*i]).getKey());
	(*i)++;
    if ((*tree_node)->right){
        putArrayIntoTree(&(*tree_node)->right, array, i);
    }
}

template<typename key_t, typename data_t>
Node<key_t, data_t>* Tree<key_t, data_t>::createEmptyTree(int* height) {
	if(*height == -1){
		(*height)++;
		return nullptr;
	}
	Node<key_t, data_t>* node = new Node<key_t, data_t>();
	Node<key_t, data_t>* temp_node;
	*(height) = *(height) - 1;
	temp_node = createEmptyTree(height);
	node->setLeft(temp_node);
	if(temp_node){
		temp_node->father = node;
	}
	*(height) = *(height) - 1;
	temp_node = createEmptyTree(height);
	if(temp_node){
		temp_node->father = node;
	}
	node->setRight(temp_node);
	node->updateHeight();
	(*height)++;
	return node;
}

template<typename key_t, typename data_t>
bool Tree<key_t, data_t>::removeExtraTreeNodes(Node<key_t, data_t> *node, int* num_of_nodes_to_delete) {
    if (*num_of_nodes_to_delete == 0 || node == nullptr){
        return false;
    }

    if (node->isLeaf() && *num_of_nodes_to_delete > 0){
        delete node;
        (*num_of_nodes_to_delete)--;
        return true;
    }

    Node<key_t, data_t>* right_son = node->getRight();
    if(right_son != nullptr){
        if (removeExtraTreeNodes(right_son, num_of_nodes_to_delete)){
            node->right = nullptr;
        }
    }

    Node<key_t, data_t>* left_son = node->getLeft();
    if(left_son != nullptr){
        if (removeExtraTreeNodes(left_son, num_of_nodes_to_delete)){
            node->left = nullptr;
        }
    }

    node->updateBF();
    node->updateHeight();
    return false;
}


template<typename key_t, typename data_t>
int Tree<key_t, data_t>::findRequiredEmptyTreeHeight(int n) {
	return ceil(log2(n+1) - 1);
}

template<typename key_t, typename data_t>
int Tree<key_t, data_t>::findNumOfNodesToDelete(int size){
	int tree_height = findRequiredEmptyTreeHeight(size);
    double num_of_nodes_to_delete = (exp2(1 + tree_height) - 1) - size;
    int num_of_nodes_to_delete_int = ceil(num_of_nodes_to_delete);
    return num_of_nodes_to_delete_int;
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::putTreeToArray(Node<key_t, data_t>* node, Node<key_t, data_t> **arr, int* i) {
	if(node == nullptr){
		return;
	}
	putTreeToArray(node->left, arr, i);
	arr[*i] = node;
	(*i)++;
	putTreeToArray(node->right, arr, i);
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::mergeArrays(Node<key_t, data_t> **arr1, Node<key_t, data_t> **arr2,
										Node<key_t, data_t> **merged_arr, int arr1_size, int arr2_size) {
	int i= 0, j = 0, k = 0;
	while(i < arr1_size && j < arr2_size){
		if (((Node<key_t, data_t>*)arr1[i])->key < ((Node<key_t, data_t>*)arr2[j])->key){
			merged_arr[k] = arr1[i];
			i++;
		}
		else {
			merged_arr[k] = arr2[j];
			j++;
		}
		k++;
	}
	if((arr1_size-i) > (arr2_size-j)){
		while(k < arr1_size + arr2_size) {
			merged_arr[k] = arr1[i];
			i++;
			k++;
		}
	}
	else{
		while(k < arr1_size + arr2_size) {
			merged_arr[k] = arr2[j];
			j++;
			k++;
		}
	}
}

template<typename key_t, typename data_t>
int Tree<key_t, data_t>::getSize() const {
	return size;
}

template<typename key_t, typename data_t>
void Tree<key_t, data_t>::getRootIndexInOrder(Node<key_t, data_t>* node, int *index_ptr) {
	if(node == nullptr){
		return;
	}
	getRootIndexInOrder(node->left,  index_ptr);
	(*index_ptr)++;
	getRootIndexInOrder(node->right,  index_ptr);
}

#endif //TREE_H
