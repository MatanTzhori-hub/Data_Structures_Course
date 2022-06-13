#ifndef RANK_TREE_H
#define RANK_TREE_H

#include "iterator.h"
#include "employee_key.h"
#include <cmath>
#include <iostream>

typedef enum {MY_ALLOCATION_ERROR, MY_INVALID_INPUT, MY_FAILURE, MY_SUCCESS, ELEMENT_EXISTS,
	ELEMENT_DOES_NOT_EXIST, NO_ELEMENT_INSERT_LEFT, NO_ELEMENT_INSERT_RIGHT, NO_ROOT} ReturnValue;


using std::ostream;

class Company;

template<typename key_t, typename data_t, typename rank_t>
class RankTree {
	Node<key_t, data_t, rank_t>* root;
	int size;

	//tree rolls
	ReturnValue fixTree(Node<key_t, data_t, rank_t> *node);
	ReturnValue RollTree(Node<key_t, data_t, rank_t>& node);
	ReturnValue LLRoll(Node<key_t, data_t, rank_t>& node);
	ReturnValue RRRoll(Node<key_t, data_t, rank_t>& node);
	ReturnValue LRRoll(Node<key_t, data_t, rank_t>& node);
	ReturnValue RLRoll(Node<key_t, data_t, rank_t>& node);

	//Node Removal Helper Functions
	ReturnValue removeRoot(Node<key_t, data_t, rank_t> *to_delete);
	ReturnValue removeNonRoot(Node<key_t, data_t, rank_t> *to_delete);

    //Node Swaps
	ReturnValue swapNodes(Node<key_t, data_t, rank_t>* node1, Node<key_t, data_t, rank_t>* node2);
	void swapRoot(Node<key_t, data_t, rank_t>* received_root, Node<key_t, data_t, rank_t>* node);
	void swapNonRoot(Node<key_t, data_t, rank_t>* node1, Node<key_t, data_t, rank_t>* node2);

    //Tree Merging Helper Function and Calculations
	int findRequiredEmptyTreeHeight(int n);
	int findNumOfNodesToDelete(int size);
	Node<key_t, data_t, rank_t>* createEmptyTree(int* height);
	bool removeExtraTreeNodes(Node<key_t, data_t, rank_t>* node, int* num_of_nodes_to_delete);

	//Array casting
	void putTreeToArray(Node<key_t, data_t, rank_t>* node, Node<key_t, data_t, rank_t>* arr[], int* i, int accum_bump);
    void putArrayIntoTree(Node<key_t, data_t, rank_t>** tree_node, Node<key_t, data_t, rank_t>*** array, int* i); 
	
	//Array merge
	void mergeArrays(Node<key_t, data_t, rank_t>* arr1[], Node<key_t, data_t, rank_t>* arr2[],
					   Node<key_t, data_t, rank_t>* merged_arr[], int arr1_size, int arr2_size);

	//Bumps calculation - trust that son is really a son of father
	long long calculateBumpsUpToFather(Node<key_t, data_t, rank_t>* son, Node<key_t, data_t, rank_t>* father);

	long long getSumGradeUpTo(Node<key_t, data_t, rank_t>* upper_bound);
	void bumpGradeUpTo(Node<key_t, data_t, rank_t>* upper_bound, long long bump_amount);

	void updateRankToAllTree(Node<key_t, data_t, rank_t>* node);

public:
	RankTree() : size(0) {root = nullptr; }
	~RankTree();

	//Add or Remove elements
	void deleteTreeNodes(Node<key_t, data_t, rank_t> *node);
	ReturnValue insert(key_t key, data_t element, rank_t rank);
	ReturnValue removeElement(key_t key);

    Node<key_t, data_t, rank_t>* findElementNode(key_t key);
	Iterator<key_t, data_t, rank_t> findElement(key_t key);
	Iterator<key_t, data_t, rank_t> findCloseestElement(key_t key);
	Iterator<key_t, data_t, rank_t> findCloseestElementAbove(key_t key);
	Iterator<key_t, data_t, rank_t> findCloseestElementBelow(key_t key);

	// bumps and grades
	void bumpGradeInRange(int lowest_salary, int highest_salary, long long bump_amount);
	void bumpSingleEmployeeGrade(Node<key_t, data_t, rank_t>* to_update, long long bump_amount);
	long long getSumGradeOfmTop(int m);
	long long getTotalSumGrade();
	long long getSumGradeInRange(int lowest_salary, int highest_salary);
	int getNumEmployeesInRange(int lowest_salary, int highest_salary);
	long long calcTrueEmployeeGrade(Node<key_t, data_t, rank_t>* employee_node);
	

	int getSize() const;
	Iterator<key_t, data_t, rank_t> begin(int order = 0);
	Iterator<key_t, data_t, rank_t> begin(int order = 0) const;
	Iterator<key_t, data_t, rank_t> end();
	Iterator<key_t, data_t, rank_t> end() const;
	Iterator<key_t, data_t, rank_t> createIterByNode(Node<key_t, data_t, rank_t>& node);
	Iterator<key_t, data_t, rank_t> createIterByNode(Node<key_t, data_t, rank_t>& node) const;
	void getRootIndexInOrder(Node<key_t, data_t, rank_t>* node, int* index_ptr);
	void getRightMostNode(Node<key_t, data_t, rank_t>** node_ptr);
	Iterator<key_t, data_t, rank_t> getRightMost();

	//Ranking
	int Rank(key_t key);
	Iterator<key_t, data_t, rank_t> Select(int r);

	// Print functions
	void printTree();
	void printInOrder(Node<key_t, data_t, rank_t>* node);
	
	// Merge other tree to this tree
	RankTree<key_t, data_t, rank_t>* mergeToMe(RankTree<key_t, data_t, rank_t>& tree2);
};

template<typename key_t, typename data_t, typename rank_t>
RankTree<key_t, data_t, rank_t>::~RankTree<key_t, data_t, rank_t>() {
	deleteTreeNodes(root);
	delete root;
}


template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::deleteTreeNodes(Node<key_t, data_t, rank_t> *node) {
	if(node == nullptr){
		return;
	}
	deleteTreeNodes(node->getLeft());
	deleteTreeNodes(node->getRight());
	if(node != root){
		node->isALeftSon() ? node->father->left = nullptr : node->father->right = nullptr;
		node->father = nullptr;
		delete node;
	}
}

template<typename key_t, typename data_t, typename rank_t>
Node<key_t, data_t, rank_t>* RankTree<key_t, data_t, rank_t>::findElementNode(key_t key){
    if(root == nullptr) {
        return nullptr;
    }

    Node<key_t, data_t, rank_t>* curr_node = root;

    while(curr_node){
        if(curr_node->key == key){
            return curr_node;
        }
        else if(curr_node->key > key){
            curr_node = curr_node->getLeft();
        }
        else{
            curr_node = curr_node->getRight();
        }
    }

    return nullptr;
}


template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::findElement(key_t key){
	if(root == nullptr) {
		return end();
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	
	while(curr_node){
		if(curr_node->key == key){
			return Iterator<key_t, data_t, rank_t>(curr_node);
		}
		else if(curr_node->key > key){
            curr_node = curr_node->getLeft();
			}
			else{
            curr_node = curr_node->getRight();
			}
		}
	
	return end();
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::findCloseestElement(key_t key){
	if(root == nullptr) {
		return end();
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	Node<key_t, data_t, rank_t>* closest = root;
	int min_diff = abs(root->key - key);

	while(curr_node){
		if(curr_node->key == key){
			return Iterator<key_t, data_t, rank_t>(curr_node);
		}
		else{
			if (min_diff > abs(curr_node->key - key))
			{ 
				min_diff = abs(curr_node->key - key);
				closest = curr_node;
			} 
			if(curr_node->key > key){
                curr_node = curr_node->getLeft();
			}
			else{
                curr_node = curr_node->getRight();
			}
		}
	}
	return Iterator<key_t, data_t, rank_t>(closest);
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::findCloseestElementAbove(key_t key){
	if(root == nullptr) {
		return end();
	}

	Node<key_t, data_t, rank_t>* curr_node = root;
	Node<key_t, data_t, rank_t>* closest = nullptr;

	while(curr_node){
		if(curr_node->key.getSalary() == key.getSalary()){
			if(curr_node->key.getId() < key.getId()){
				closest = curr_node;
				curr_node = curr_node->getRight();
			}
			else{
				curr_node = curr_node->getLeft();
			}
		}
		else if(curr_node->key.getSalary() > key.getSalary()){
			if(closest != nullptr && curr_node->key.getSalary() > closest->key.getSalary()){
				curr_node = curr_node->getLeft();
			}
			else{
				closest = curr_node;
				curr_node = curr_node->getLeft();
			}
		}
		else{
			curr_node = curr_node->getRight();
		}
	}
	
	return Iterator<key_t, data_t, rank_t>(closest);
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::findCloseestElementBelow(key_t key){
	if(root == nullptr) {
		return end();
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	Node<key_t, data_t, rank_t>* closest = nullptr;

	while(curr_node){
		if(curr_node->key.getSalary() == key.getSalary()){
			if(curr_node->key.getId() > key.getId()){
				closest = curr_node;
				curr_node = curr_node->getLeft();
			}
			else{
				curr_node = curr_node->getRight();
			}
		}
		else if(curr_node->key.getSalary() < key.getSalary()){
			if(closest != nullptr && curr_node->key.getSalary() < closest->key.getSalary()){
				curr_node = curr_node->getRight();
			}
			else{
				closest = curr_node;
				curr_node = curr_node->getRight();
			}
		}
		else{
			curr_node = curr_node->getLeft();
		}
	}

	return Iterator<key_t, data_t, rank_t>(closest);
}

template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::getSumGradeOfmTop(int m){
	long long sum_grade = getTotalSumGrade();
	Node<key_t, data_t, rank_t>* lower_bound = Select(size - m).getNodePtr();
	sum_grade -= getSumGradeUpTo(lower_bound);
	return sum_grade;
}


template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::getTotalSumGrade(){
	return root->rank.getSumOfGrades();
}


template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::getSumGradeUpTo(Node<key_t, data_t, rank_t>* upper_bound){
	Node<key_t, data_t, rank_t>* curr_node = root;
	long long sum_grades = 0;
	long long accumulative_bump = 0;
	while(curr_node != upper_bound){
		accumulative_bump += curr_node->rank.getGradeBump();
		if(curr_node->key < upper_bound->key){
			if(curr_node->left != nullptr){
				sum_grades += curr_node->left->rank.getSumOfGrades();
				sum_grades += curr_node->left->rank.weight * accumulative_bump;
			}
			sum_grades += curr_node->rank.getTreeGrade();
			sum_grades += accumulative_bump;
		}
		
		if(curr_node->key == upper_bound->key){
			break;
		}
		else if(curr_node->key > upper_bound->key){
            curr_node = curr_node->getLeft();
		}
		else{
            curr_node = curr_node->getRight();
		}
	}
	accumulative_bump += curr_node->rank.getGradeBump();

	if(curr_node->left != nullptr){
		sum_grades += curr_node->left->rank.getSumOfGrades();
		sum_grades += curr_node->left->rank.weight * accumulative_bump;
	}
	sum_grades += curr_node->rank.getTreeGrade();
	sum_grades += accumulative_bump;

	return sum_grades;
}

template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::getSumGradeInRange(int lowest_salary, int highest_salary){
	long long sum_grades = 0;

	EmployeeKey temp_highest_key = EmployeeKey(0, highest_salary);
	Iterator<key_t, data_t, rank_t> upper_bound_iter = findCloseestElementBelow(temp_highest_key);
	if(upper_bound_iter == end()){
		return 0;
	}

	if(upper_bound_iter.getKey().getSalary() == highest_salary){
		temp_highest_key.setId(MAX_ID);
		upper_bound_iter = findCloseestElementAbove(temp_highest_key);
	}
	sum_grades += getSumGradeUpTo(upper_bound_iter.getNodePtr());


	EmployeeKey temp_lowest_key = EmployeeKey(0, lowest_salary);
	Iterator<key_t, data_t, rank_t> lower_bound_iter = findCloseestElementBelow(temp_lowest_key);
	if(lower_bound_iter == end()){
		return sum_grades;
	}
	else{
		sum_grades -= getSumGradeUpTo(lower_bound_iter.getNodePtr());
		if(lower_bound_iter.getKey().getSalary() >= lowest_salary && lower_bound_iter.getKey().getSalary() <= highest_salary){
			sum_grades += calcTrueEmployeeGrade(lower_bound_iter.getNodePtr());
		}
	}

	return sum_grades;
}

template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::calcTrueEmployeeGrade(Node<key_t, data_t, rank_t>* employee_node){
	if(employee_node == nullptr){
		return -1;
	}

	long long total_grade = employee_node->rank.getTreeGrade() + employee_node->rank.getGradeBump();
	Node<key_t, data_t, rank_t>* curr_node = employee_node->getFather();
	while(curr_node){
		total_grade += curr_node->rank.getGradeBump();
		curr_node = curr_node->father;
	}

	return total_grade;
}


template<typename key_t, typename data_t, typename rank_t>
int RankTree<key_t, data_t, rank_t>::getNumEmployeesInRange(int lowest_salary, int highest_salary){

	EmployeeKey temp_highest_key = EmployeeKey(0, highest_salary);
	Iterator<key_t, data_t, rank_t> upper_bound_iter = findCloseestElementBelow(temp_highest_key);
	if(upper_bound_iter == end()){
		return 0;
	}
	else if(upper_bound_iter.getKey().getSalary() == highest_salary){
		temp_highest_key.setId(MAX_ID);
		upper_bound_iter = findCloseestElementAbove(temp_highest_key);
	}
	else{
		if(upper_bound_iter.getKey().getSalary() < lowest_salary)
		return 0;
	}

	EmployeeKey temp_lowest_key = EmployeeKey(MAX_ID, lowest_salary);
	Iterator<key_t, data_t, rank_t> lower_bound_iter = findCloseestElementAbove(temp_lowest_key);
	if(lower_bound_iter == end()){
		return 0;
	}
	else if(lower_bound_iter.getKey().getSalary() == lowest_salary){
		temp_lowest_key.setId(0);
		lower_bound_iter = findCloseestElementBelow(temp_lowest_key);
	}
	else{
		if(lower_bound_iter.getKey().getSalary() > highest_salary)
		return 0;
	}
	
	int upper_rank = Rank(upper_bound_iter.getKey());
	int lower_rank = Rank(lower_bound_iter.getKey());


	return upper_rank - lower_rank + 1;
}


//todo : finish this function!
template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::bumpGradeUpTo(Node<key_t, data_t, rank_t>* upper_bound, long long bump_amount){
	// enum Crossed {IN, OUT, NO};
	// Crossed status = NO;

	Node<key_t, data_t, rank_t>* prev = root;
	Node<key_t, data_t, rank_t>* curr = prev;

	//if we begin by going right, update the root with the bump
	if(curr->getKey() < upper_bound->getKey()){
		curr->rank.setGradeBump(curr->rank.getGradeBump() + bump_amount);
	}

	while(curr != upper_bound){
		if(curr->getKey() < upper_bound->getKey()){
			prev = curr;
			curr = curr->right;
		}
		else{
			prev = curr;
			curr = curr->left;
		}

		//crossed out
		if(prev->getKey() < upper_bound->getKey() && curr->getKey() > upper_bound->getKey()){
			curr->rank.setGradeBump(curr->rank.getGradeBump() - bump_amount);
		}
		//crossed in
		else if(prev->getKey() > upper_bound->getKey() && curr->getKey() <= upper_bound->getKey()){
			curr->rank.setGradeBump(curr->rank.getGradeBump() + bump_amount);
		}
	}

	if(curr->right != nullptr){
		curr->right->rank.setGradeBump(curr->right->rank.getGradeBump() - bump_amount);
		fixTree(curr->right);
	}
	else{
		fixTree(curr);
	}
}


template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::bumpGradeInRange(int lowest_salary, int highest_salary, long long bump_amount){
	
	EmployeeKey temp_highest_key = EmployeeKey(0, highest_salary);
	Iterator<key_t, data_t, rank_t> upper_bound_iter = findCloseestElementBelow(temp_highest_key);
	if(upper_bound_iter == end()){
		return;
	}

	if(upper_bound_iter.getKey().getSalary() == highest_salary){
		temp_highest_key.setId(MAX_ID);
		upper_bound_iter = findCloseestElementAbove(temp_highest_key);
	}
	bumpGradeUpTo(upper_bound_iter.getNodePtr(), bump_amount);


	EmployeeKey temp_lowest_key = EmployeeKey(0, lowest_salary - 1);
	Iterator<key_t, data_t, rank_t> lower_bound_iter = findCloseestElementBelow(temp_lowest_key);
	if(lower_bound_iter == end()){
		return;
	}

	if(lower_bound_iter.getKey().getSalary() == lowest_salary - 1){
		temp_lowest_key.setId(MAX_ID);
		lower_bound_iter = findCloseestElementAbove(temp_lowest_key);
	}
	bumpGradeUpTo(lower_bound_iter.getNodePtr(), -1 * bump_amount);
	
}


//todo : add function for promoting grade for specific employee, and then fix the tree
template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::bumpSingleEmployeeGrade(Node<key_t, data_t, rank_t>* to_update, long long bump_amount){
	to_update->rank.setTreeGrade(to_update->rank.getTreeGrade() + bump_amount);
	fixTree(to_update);
}



template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::insert(key_t key, data_t element, rank_t rank) {
	auto* node_to_insert = new Node<key_t, data_t, rank_t>(key,element,rank);	//this here is roy and adi fix and is important
	if(!node_to_insert){
		return MY_ALLOCATION_ERROR;
	}

	if(!root){
		size++;
		root = node_to_insert;
		return MY_SUCCESS;
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	bool found = false;
	long long tot_grade_bump_on_the_way = curr_node->rank.getGradeBump();
	while(curr_node && !found){
		if(curr_node->key == key){
			delete node_to_insert;
			return ELEMENT_EXISTS;
		}
		else if(curr_node->key > key){
				if(curr_node->getLeft() != nullptr){
                    curr_node = curr_node->getLeft();
					tot_grade_bump_on_the_way += curr_node->rank.getGradeBump();
				}
				else{
					found = true;
                    curr_node->left = node_to_insert;
					node_to_insert->father = curr_node;
					node_to_insert->rank.setGradeBump(node_to_insert->rank.getGradeBump() - tot_grade_bump_on_the_way);
					node_to_insert->updateRank();
				}
			}
			else{
				if(curr_node->getRight() != nullptr){
                    curr_node = curr_node->getRight();
					tot_grade_bump_on_the_way += curr_node->rank.getGradeBump();
				}
				else{
					found = true;
                    curr_node->right = node_to_insert;
					node_to_insert->father = curr_node;
					node_to_insert->rank.setGradeBump(node_to_insert->rank.getGradeBump() - tot_grade_bump_on_the_way);
					node_to_insert->updateRank();
				}
			}
		}

	size++;
	return fixTree(curr_node);
}


template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::fixTree(Node<key_t, data_t, rank_t> *node){
	int old_height = 0;
	while(node != nullptr){
		//old_height = node->height;
		node->updateHeight();
		node->updateBF();
		node->updateRank();
		if(abs(node->balance_factor) == 2){
			if(RollTree(*node) == MY_ALLOCATION_ERROR) {
				return MY_ALLOCATION_ERROR;
			}
			//node->updateRank();
			node = node->getFather();
		}
        if(node == nullptr){
            break;
        }
		// Errored out - because we want to keep going to the root to update the rank and the sum_grades
        // if(old_height == node->height){
		// 	break;
		// }
		node = node->getFather();
	}
	return MY_SUCCESS;
}

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::RollTree(Node<key_t, data_t, rank_t>& node) {
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

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::LLRoll(Node<key_t, data_t, rank_t>& node) {
	Node<key_t, data_t, rank_t>* saved_left_ptr = node.left;
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
	if(node.left){
		node.left->rank.setGradeBump(node.left->rank.getGradeBump() + saved_left_ptr->rank.getGradeBump());
	}
	saved_left_ptr->rank.setGradeBump(saved_left_ptr->rank.getGradeBump() + node.rank.getGradeBump());
	node.rank.setGradeBump(node.rank.getGradeBump() - saved_left_ptr->rank.getGradeBump());
	node.updateRank();
	saved_left_ptr->updateRank();
	saved_left_ptr->updateHeight();
	saved_left_ptr->updateBF();
	return MY_SUCCESS;
}


template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::RRRoll(Node<key_t, data_t, rank_t> &node) {
	Node<key_t, data_t, rank_t>* saved_right_ptr = node.right;
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
	if(node.right){
		node.right->rank.setGradeBump(node.right->rank.getGradeBump() + saved_right_ptr->rank.getGradeBump());
	}
	saved_right_ptr->rank.setGradeBump(saved_right_ptr->rank.getGradeBump() + node.rank.getGradeBump());
	node.rank.setGradeBump(node.rank.getGradeBump() - saved_right_ptr->rank.getGradeBump());
	node.updateRank();
	saved_right_ptr->updateRank();
	saved_right_ptr->updateHeight();
	saved_right_ptr->updateBF();
	return MY_SUCCESS;
}


template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::LRRoll(Node<key_t, data_t, rank_t> &node) {
	if(RRRoll(*(node.left)) == MY_SUCCESS){
		if (LLRoll(node) == MY_SUCCESS){
			return MY_SUCCESS;
		}
	}
	return MY_ALLOCATION_ERROR;
}

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::RLRoll(Node<key_t, data_t, rank_t> &node) {
	if(LLRoll(*(node.right)) == MY_SUCCESS){
		if(RRRoll(node) == MY_SUCCESS){
			return MY_SUCCESS;
		}
	}
	return MY_ALLOCATION_ERROR;
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::begin(int order) {
	if(root == nullptr){
		return end();
	}
	Node<key_t, data_t, rank_t> *curr = root;
	if(order >= 0 ){
		while(curr->getLeft() != nullptr){
            curr = curr->getLeft();
		}
	}
	else{
		while(curr->getRight() != nullptr){
            curr = curr->getRight();
		}
	}
	return Iterator<key_t, data_t, rank_t>(curr, order);
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::begin(int order) const {
	if(root == nullptr){
		return end();
	}
	Node<key_t, data_t, rank_t> *curr = root;
	while(curr->getLeft() != nullptr){
        curr = curr->getLeft();
	}
	return Iterator<key_t, data_t, rank_t>(curr);
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::end() {
	return Iterator<key_t, data_t, rank_t>(nullptr);
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::end() const {
	return Iterator<key_t, data_t, rank_t>(nullptr);
}

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::removeElement(key_t key) {
	Iterator<key_t, data_t, rank_t> iter = findElement(key);

	if(iter == end()){
		return MY_FAILURE;
	}
	else{
		Node<key_t, data_t, rank_t> *node = iter.ptr;
		node->data->setGrade(calcTrueEmployeeGrade(node));
		if(node == root){
			return removeRoot(node);
		}
		else{
			return removeNonRoot(node);
		}
	}
}

// calculate the sum of bump without the father and the son
template<typename key_t, typename data_t, typename rank_t>
long long RankTree<key_t, data_t, rank_t>::calculateBumpsUpToFather(Node<key_t, data_t, rank_t>* son, Node<key_t, data_t, rank_t>* father){
	if(son == nullptr || father == nullptr){
		return -1;
	}
	
	long long sum_bumps = 0;
	son = son->father;
	while(son != father){
		sum_bumps += son->rank.getGradeBump();
		son = son->father;
	}
	return sum_bumps;
}

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::removeRoot(Node<key_t, data_t, rank_t> *to_delete) {
	if(!to_delete){
		return MY_ALLOCATION_ERROR;
	}
	if(to_delete->isLeaf()){
		root = nullptr;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else if(to_delete->onlyHaveRightSon()) {
		to_delete->right->rank.setGradeBump(to_delete->right->rank.getGradeBump() + to_delete->rank.getGradeBump());
		to_delete->right->father = nullptr;
		root = to_delete->right;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else if(to_delete->onlyHaveLeftSon()){
		to_delete->left->rank.setGradeBump(to_delete->left->rank.getGradeBump() + to_delete->rank.getGradeBump());
		to_delete->left->father = nullptr;
		root = to_delete->left;
		delete to_delete;
		size--;
		return MY_SUCCESS;
	}
	else{
		Node<key_t, data_t, rank_t> *to_replace = to_delete->getRight();
		while(to_replace->left != nullptr){
			to_replace = to_replace->getLeft();
		}

		// check if to_replace is direct son of to_delete, and update grade_bump accordingly
		if(to_delete->right == to_replace){
			to_replace->rank.setGradeBump(to_replace->rank.getGradeBump() + to_delete->rank.getGradeBump());
		}
		else{
			int bumps_until_to_delete = calculateBumpsUpToFather(to_replace, to_delete);
			to_replace->rank.setTreeGrade(to_replace->rank.getTreeGrade() + to_replace->rank.getGradeBump() + bumps_until_to_delete);
			int temp_bump = to_replace->rank.getGradeBump();
			to_replace->rank.setGradeBump(to_delete->rank.getGradeBump());
			to_delete->rank.setGradeBump(temp_bump);
		}


		if(swapNodes(to_delete, to_replace) == MY_ALLOCATION_ERROR){
			return MY_ALLOCATION_ERROR;
		}
		return removeNonRoot(to_delete);
	}
}

template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::removeNonRoot(Node<key_t, data_t, rank_t> *to_delete) {
	if(!to_delete){
		return MY_ALLOCATION_ERROR;
	}
	if(to_delete->haveTwoSons()){
		Node<key_t, data_t, rank_t> *to_replace = to_delete->getRight();
		while(to_replace->left){
			to_replace = to_replace->getLeft();
		}

		// check if to_replace is son of to_delete, and update grade_bump accordingly
		if(to_delete->right == to_replace){
			to_replace->rank.setGradeBump(to_replace->rank.getGradeBump() + to_delete->rank.getGradeBump());
		}
		else{
			int bumps_until_to_delete = calculateBumpsUpToFather(to_replace, to_delete);
			to_replace->rank.setTreeGrade(to_replace->rank.getTreeGrade() + to_replace->rank.getGradeBump() + bumps_until_to_delete);
			int temp_bump = to_replace->rank.getGradeBump();
			to_replace->rank.setGradeBump(to_delete->rank.getGradeBump());
			to_delete->rank.setGradeBump(temp_bump);
		}

		if(swapNodes(to_delete, to_replace) == MY_ALLOCATION_ERROR){
			return MY_ALLOCATION_ERROR;
		}
	}

	//Iterator<key_t, data_t, rank_t> new_iter = createIterByNode(*to_delete);

	if(to_delete->isLeaf()){
		if(to_delete->isALeftSon()){
			to_delete->father->left = nullptr;
		}
		else if(to_delete->isARightSon()){
			to_delete->father->right = nullptr;
		}
		Node<key_t, data_t, rank_t> *parent = to_delete->getFather();
		delete to_delete;
		size--;
		return fixTree(parent);
	}
	if(to_delete->onlyHaveLeftSon()){
		to_delete->left->rank.setGradeBump(to_delete->left->rank.getGradeBump() + to_delete->rank.getGradeBump());
		to_delete->left->updateRank();
		if(to_delete->isALeftSon()){
			to_delete->father->left = to_delete->left;
			to_delete->left->father = to_delete->father;
			Node<key_t, data_t, rank_t> *parent = to_delete->getFather();
            delete to_delete;
			size--;
			return fixTree(parent);
		} else if(to_delete->isARightSon()){
			to_delete->father->right = to_delete->left;
			to_delete->left->father = to_delete->father;
			Node<key_t, data_t, rank_t> *parent = to_delete->getFather();
            delete to_delete;
			size--;
			return fixTree(parent);
		}
	}
	else if(to_delete->onlyHaveRightSon()){
		to_delete->right->rank.setGradeBump(to_delete->right->rank.getGradeBump() + to_delete->rank.getGradeBump());
		to_delete->right->updateRank();
		if(to_delete->isALeftSon()){
			to_delete->father->left = to_delete->right;
			to_delete->right->father = to_delete->father;
			Node<key_t, data_t, rank_t> *parent = to_delete->getFather();
            delete to_delete;
			size--;
			return fixTree(parent);
		} else if(to_delete->isARightSon()){
			to_delete->father->right = to_delete->right;
			to_delete->right->father = to_delete->father;
			Node<key_t, data_t, rank_t> *parent = to_delete->getFather();
            delete to_delete;
			size--;
			return fixTree(parent);
		}
	}
	return MY_SUCCESS;
}


/*
 * Assumes the node to be deleted is node1, and the other one is node2
 */
template<typename key_t, typename data_t, typename rank_t>
ReturnValue RankTree<key_t, data_t, rank_t>::swapNodes(Node<key_t, data_t, rank_t>* node1, Node<key_t, data_t, rank_t>* node2) {
	if(root == node1){
		swapRoot(node1, node2);
	}
	else{
		swapNonRoot(node1, node2);
	}
	return MY_SUCCESS;
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::createIterByNode(Node<key_t, data_t, rank_t> &node) {
	return Iterator<key_t, data_t, rank_t>(&node);
}


template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::createIterByNode(Node<key_t, data_t, rank_t> &node) const {
	return Iterator<key_t, data_t, rank_t>(&node);
}


using std::cout;

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::printTree() {
	printInOrder(root);
}

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::printInOrder(Node<key_t, data_t, rank_t>* node) {
	if(node == nullptr){
		return;
	}
	printInOrder(node->left);
	node->printNode();
	printInOrder(node->right);

}

// template<typename key_t, typename data_t, typename rank_t>
// void RankTree<key_t, data_t, rank_t>::getRightMostNode(Node<key_t, data_t, rank_t>** node_ptr){
// 	Iterator<key_t, data_t, rank_t> iter = begin();
// 	if(iter.ptr == nullptr) {
// 		node_ptr = nullptr;
// 	}
// 	Iterator<key_t, data_t, rank_t> temp_iter = begin();
// 	if(temp_iter.ptr == nullptr){
// 		node_ptr = nullptr;
// 	}
// 	while(temp_iter.ptr){
// 		if(iter.ptr->getRight() == nullptr){
// 			*node_ptr = iter.getPtr();
// 			return;
// 		}
// 		iter.goRight();
// 		temp_iter.goRight();
// 	}
// 	*node_ptr = iter.getPtr();
// }

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::getRightMost(){
	if(root == nullptr) {
		return Iterator<key_t, data_t, rank_t>(root);
	}
	Node<key_t, data_t, rank_t> *node = root;
	while(node->getRight() != nullptr){
		node = node->getRight();
	}

	return Iterator<key_t, data_t, rank_t>(node);
}

template<typename key_t, typename data_t, typename rank_t>
int RankTree<key_t, data_t, rank_t>::Rank(key_t key){
	int r = 0;

	if(root == nullptr) {
		return -1;
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	
	while(curr_node){
		if(curr_node->key == key){
			if(curr_node->left != nullptr){
				r += curr_node->left->rank.getWeight();
			}
			r += 1;
			return r;
		}
		else if(curr_node->key > key){
            	curr_node = curr_node->getLeft();
			}
			else{
				if(curr_node->left != nullptr){
					r += curr_node->left->rank.getWeight();
				}
				r += 1;
            	curr_node = curr_node->getRight();
			}
		}
	
	return -1;
}

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t> RankTree<key_t, data_t, rank_t>::Select(int r){
	if(root == nullptr) {
		return end();
	}
	
	Node<key_t, data_t, rank_t>* curr_node = root;
	
	while(curr_node){
		if(curr_node->left == nullptr){
			if(0 == (r - 1)){
				return Iterator<key_t, data_t, rank_t>(curr_node);
			}
			else{
				r = r - 1;
				curr_node = curr_node->getRight();
			}
		}
		else{
			if(curr_node->left->rank.getWeight() == (r - 1)){
				return Iterator<key_t, data_t, rank_t>(curr_node);
			}
			else if(curr_node->left->rank.getWeight() > (r - 1)){
					curr_node = curr_node->getLeft();
				}
				else{
					r = r - curr_node->left->rank.getWeight() - 1;
					curr_node = curr_node->getRight();
				}
			}
		}
		
	
	return end();
}

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::swapRoot(Node<key_t, data_t, rank_t> *received_root, Node<key_t, data_t, rank_t> *node) {
	Node<key_t, data_t, rank_t>* orig_father_of_node = node->father;
	Node<key_t, data_t, rank_t>* orig_right_son_of_node = node->right;
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

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::swapNonRoot(Node<key_t, data_t, rank_t> *node1, Node<key_t, data_t, rank_t> *node2) {
	Node<key_t, data_t, rank_t>* orig_father_of_node2 = node2->father;
	Node<key_t, data_t, rank_t>* orig_right_son_of_node2 = node2->right;
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

template<typename key_t, typename data_t, typename rank_t>
RankTree<key_t, data_t, rank_t>* RankTree<key_t, data_t, rank_t>::mergeToMe(RankTree<key_t, data_t, rank_t>& other_tree) {
	if (other_tree.getSize() == 0){
        return this;
    }

	/*
	 * create arrays for each tree, and merged tree
	 * */
	Node<key_t, data_t, rank_t>** arr1 = new Node<key_t, data_t, rank_t>*[sizeof(Node<key_t, data_t, rank_t>*) * getSize()];
	Node<key_t, data_t, rank_t>** arr2 = new Node<key_t, data_t, rank_t>*[sizeof(Node<key_t, data_t, rank_t>*) * other_tree.getSize()];
	Node<key_t, data_t, rank_t>** merged_arr = new Node<key_t, data_t, rank_t>*[sizeof(Node<key_t, data_t, rank_t>*) * (getSize()+other_tree.getSize())];

	/*
	 * insert all nodes of each tree to it's array
	 * */
	int i=0 , j = 0;
	this->putTreeToArray(root, arr1, &i, 0);
	other_tree.putTreeToArray(other_tree.root, arr2, &j, 0);

	/*
	 * merge the 2 arrays to 1 array
	 * */
	mergeArrays(arr1, arr2, merged_arr, size, other_tree.getSize());

	/*
	 * create new tree with new size
	 * */
	int merged_size = size + other_tree.getSize();
	int req_tree_height = findRequiredEmptyTreeHeight(merged_size);
	Node<key_t, data_t, rank_t>* new_root = createEmptyTree(&req_tree_height);
	int num_of_nodes_to_delete_int = findNumOfNodesToDelete(merged_size);
	removeExtraTreeNodes(new_root, &num_of_nodes_to_delete_int);

	int m=0;
	Node<key_t, data_t, rank_t>* old_root = this->root;
	this->root = new_root;
    putArrayIntoTree(&new_root, &merged_arr, &m);
	this->size = merged_size;

	Node<key_t, data_t, rank_t>* itr_node = root;
	updateRankToAllTree(itr_node);

	// This will be destructed when we exit this function. will destroy old nodes.
	RankTree<key_t, data_t, rank_t> temp_tree = RankTree<key_t, data_t, rank_t>();
	temp_tree.root = old_root;

    // This will be destructed when we exit this function. will destroy old nodes for the other tree.
    RankTree<key_t, data_t, rank_t> temp2_tree = RankTree<key_t, data_t, rank_t>();
    temp2_tree.root = other_tree.root;
    other_tree.root = nullptr;
    other_tree.size = 0;

	delete[] arr1;
    delete[] arr2;
    delete[] merged_arr;

	return this;
}


template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::updateRankToAllTree(Node<key_t, data_t, rank_t>* node){
	if(node == nullptr){
		return;
	}

	updateRankToAllTree(node->left);
	updateRankToAllTree(node->right);

	node->updateRank();
}


template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::putArrayIntoTree(Node<key_t, data_t, rank_t>** tree_node, Node<key_t, data_t, rank_t>*** array, int* i) {
	if (!(*tree_node)) {
		return;
	}

    if((*tree_node)->left){
        putArrayIntoTree(&(*tree_node)->left, array, i);
    }

    (*tree_node)->updateData((*(*array)[*i]).getData());
    (*tree_node)->updateKey((*(*array)[*i]).getKey());
	(*tree_node)->rank = (*(*array)[*i]).rank;
	(*i)++;
    if ((*tree_node)->right){
        putArrayIntoTree(&(*tree_node)->right, array, i);
    }
}


template<typename key_t, typename data_t, typename rank_t>
Node<key_t, data_t, rank_t>* RankTree<key_t, data_t, rank_t>::createEmptyTree(int* height) {
	if(*height == -1){
		(*height)++;
		return nullptr;
	}
	Node<key_t, data_t, rank_t>* node = new Node<key_t, data_t, rank_t>();
	Node<key_t, data_t, rank_t>* temp_node;
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

template<typename key_t, typename data_t, typename rank_t>
bool RankTree<key_t, data_t, rank_t>::removeExtraTreeNodes(Node<key_t, data_t, rank_t> *node, int* num_of_nodes_to_delete) {
    if (*num_of_nodes_to_delete == 0 || node == nullptr){
        return false;
    }

    if (node->isLeaf() && *num_of_nodes_to_delete > 0){
        delete node;
        (*num_of_nodes_to_delete)--;
        return true;
    }

    Node<key_t, data_t, rank_t>* right_son = node->getRight();
    if(right_son != nullptr){
        if (removeExtraTreeNodes(right_son, num_of_nodes_to_delete)){
            node->right = nullptr;
        }
    }

    Node<key_t, data_t, rank_t>* left_son = node->getLeft();
    if(left_son != nullptr){
        if (removeExtraTreeNodes(left_son, num_of_nodes_to_delete)){
            node->left = nullptr;
        }
    }

    node->updateBF();
    node->updateHeight();
    return false;
}


template<typename key_t, typename data_t, typename rank_t>
int RankTree<key_t, data_t, rank_t>::findRequiredEmptyTreeHeight(int n) {
	return ceil(log2(n+1) - 1);
}

template<typename key_t, typename data_t, typename rank_t>
int RankTree<key_t, data_t, rank_t>::findNumOfNodesToDelete(int size){
	int tree_height = findRequiredEmptyTreeHeight(size);
    double num_of_nodes_to_delete = (exp2(1 + tree_height) - 1) - size;
    int num_of_nodes_to_delete_int = ceil(num_of_nodes_to_delete);
    return num_of_nodes_to_delete_int;
}

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::putTreeToArray(Node<key_t, data_t, rank_t>* node, Node<key_t, data_t, rank_t> **arr, int* i, int accum_bump) {
	if(node == nullptr){
		return;
	}
	accum_bump += node->rank.getGradeBump();
	node->rank.setTreeGrade(node->rank.getTreeGrade() + accum_bump);
	node->rank.setGradeBump(0);

	putTreeToArray(node->left, arr, i, accum_bump);
	arr[*i] = node;
	(*i)++;
	putTreeToArray(node->right, arr, i, accum_bump);
}

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::mergeArrays(Node<key_t, data_t, rank_t> **arr1, Node<key_t, data_t, rank_t> **arr2,
										Node<key_t, data_t, rank_t> **merged_arr, int arr1_size, int arr2_size) {
	int i= 0, j = 0, k = 0;
	while(i < arr1_size && j < arr2_size){
		if (((Node<key_t, data_t, rank_t>*)arr1[i])->key < ((Node<key_t, data_t, rank_t>*)arr2[j])->key){
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

template<typename key_t, typename data_t, typename rank_t>
int RankTree<key_t, data_t, rank_t>::getSize() const {
	return size;
}

template<typename key_t, typename data_t, typename rank_t>
void RankTree<key_t, data_t, rank_t>::getRootIndexInOrder(Node<key_t, data_t, rank_t>* node, int *index_ptr) {
	if(node == nullptr){
		return;
	}
	getRootIndexInOrder(node->left,  index_ptr);
	(*index_ptr)++;
	getRootIndexInOrder(node->right,  index_ptr);
}

#endif //RANK_TREE_H
