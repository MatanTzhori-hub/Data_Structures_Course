#ifndef ITERATOR_H
#define ITERATOR_H

#include "node.h"

template<typename key_t, typename data_t> class Tree;

template<typename key_t, typename data_t>
class Iterator {
	Node<key_t, data_t>* ptr;
	friend class Tree<key_t, data_t>;

public:
	//Iterator() : ptr(nullptr) {}
	explicit Iterator(Node<key_t, data_t>* node_ptr) noexcept: ptr(node_ptr) {}
	~Iterator() {}
	void goRight() {ptr = ptr->getRight(); }
	void goLeft() {ptr = ptr->getLeft(); }
	void goFather() {ptr = ptr->getFather(); }
	data_t getData() const {return ptr->getData(); }
	key_t getKey() const {return ptr->getKey(); }
	Node<key_t, data_t>* getPtr() {return ptr; }
	void setPtr(Node<key_t, data_t>* new_ptr) { ptr = new_ptr; }
	bool checkNullLeft() { return (node_ptr->left == nullptr); }
    bool checkNullRight() { return (node_ptr->right == nullptr); }
};


#endif //ITERATOR_H
