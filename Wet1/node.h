#ifndef NODE_H
#define NODE_H

#include <algorithm>
#include <iostream>

template<typename key_t, typename data_t> class Tree;
template<typename key_t, typename data_t> class Iterator;

using std::max;
using std::ostream;

template<typename key_t, typename data_t>
class Node {
protected:
	key_t key;
	data_t data;
	int balance_factor;
	int height;
	Node* left;
	Node* right;
	Node* father;
public:
	Node() = default;
	~Node();
	Node(key_t key, data_t data);
	Node* getLeft() {return left; }
	Node* getRight() {return right; }
	Node* getFather() {return father; }
	data_t getData() {return data; }
	data_t* getDataPtr() {return &data; }
	key_t getKey() const {return key; }
	int getBalanceFactor() {return balance_factor; }
	int getHeight() {return height; }
	void setLeft(Node<key_t, data_t>* new_left) { left = new_left; }
	void setRight(Node<key_t, data_t>* new_right) { right = new_right; }
	void updateKey(key_t new_key);
    void updateKeyByPtr(key_t* new_key);
	void updateData(data_t new_data);
	void updateDataByPtr(data_t* new_data);
	void updateHeight();
	void updateBF();
	Node<key_t, data_t>& operator=(const Node<key_t, data_t>& node2);
	bool operator>(const Node<key_t, data_t>& node1){
		return getKey() > node1.key;
	}
	bool operator<(const Node<key_t, data_t>& node2);
	bool operator==(const Node<key_t, data_t>& node2);
	bool operator!=(const Node<key_t, data_t>& node2);
	bool isLeaf();
	bool onlyHaveLeftSon();
	bool onlyHaveRightSon();
	bool haveTwoSons();
	bool isALeftSon();
	bool isARightSon();
	void printNode();
	friend class Tree<key_t, data_t>;
	friend class Iterator<key_t, data_t>;
};


template<typename key_t, typename data_t>
Node<key_t,data_t>::Node(key_t key, data_t data) : key(key), data(data),  balance_factor(0), height(0),
													left(nullptr), right(nullptr), father(nullptr){
}


template<typename key_t, typename data_t>
Node<key_t, data_t>::~Node<key_t, data_t>() {
	left = nullptr;
	right = nullptr;
	father = nullptr;
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateHeight() {
	int left_height = (left == nullptr) ? -1 : left->height;
	int right_height = (right == nullptr) ? -1 : right->height;

	height = 1 + max(left_height, right_height);
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateBF() {
	int left_height = (left == nullptr) ? -1 : left->height;
	int right_height = (right == nullptr) ? -1 : right->height;
	balance_factor = left_height - right_height;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::isLeaf() {
	if(!right && !left){
		return true;
	}
	return false;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::onlyHaveLeftSon() {
	if(!right && left){
		return true;
	}
	return false;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::onlyHaveRightSon() {
	if(right && !left){
		return true;
	}
	return false;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::haveTwoSons(){
	if(right && left){
		return true;
	}
	return false;
}


template<typename key_t, typename data_t>
bool Node<key_t, data_t>::isALeftSon() {
	if(father == nullptr){
		return false;
	}
	if(!father->left){
		return false;
	}
	if(key == father->left->key ){
		return true;
	}
	return false;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::isARightSon() {
	if(father == nullptr){
		return false;
	}
	if(!father->right){
		return false;
	}
	if(father->right->key == key){
		return true;
	}
	return false;
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateKey(key_t new_key){
	key = new_key;
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateKeyByPtr(key_t* new_key){
	key = *new_key;
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateData(data_t new_data){
	data = new_data;
}

template<typename key_t, typename data_t>
void Node<key_t, data_t>::updateDataByPtr(data_t* new_data){
	data = *new_data;
}


template<typename key_t, typename data_t>
void Node<key_t, data_t>::printNode() {
	std::cout << "ID: " << key << " BF: " << balance_factor << " Height: " << height << std::endl;
}




template<typename key_t, typename data_t>
bool Node<key_t, data_t>::operator<(const Node<key_t, data_t>& node2){
	return (this->getKey() < node2.getKey());
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::operator==(const Node<key_t, data_t>& node2){
	return this->getKey() == node2.key;
}

template<typename key_t, typename data_t>
bool Node<key_t, data_t>::operator!=(const Node<key_t, data_t>& node2){
	return (this->getKey() != node2.key);
}

template<typename key_t, typename data_t>
Node<key_t, data_t>& Node<key_t, data_t>::operator=(const Node<key_t, data_t> &node2) {
	key = node2.key;
	data = node2.data;
	balance_factor = node2.balance_factor;
	height = node2.height;
	left = node2.left;
	right = node2.right;
	father = node2.father;
}



#endif //NODE_H
