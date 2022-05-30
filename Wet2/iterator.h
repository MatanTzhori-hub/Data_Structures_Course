#ifndef ITERATOR_H
#define ITERATOR_H

#include "node.h"

template<typename key_t, typename data_t, typename rank_t> class Tree;

template<typename key_t, typename data_t, typename rank_t>
class Iterator {
	Node<key_t, data_t, rank_t>* ptr;
	friend class Tree<key_t, data_t, rank_t>;
	int order;

public:
	//Iterator() : ptr(nullptr) {}
	explicit Iterator(Node<key_t, data_t, rank_t>* node_ptr, int order = 0) noexcept: ptr(node_ptr), order(order) {}
	~Iterator() {}
	data_t getData() const {return ptr->getData(); }
	key_t getKey() const {return ptr->getKey(); }
    Node<key_t, data_t, rank_t>* getNodePtr() { return ptr; }

	Iterator& next();
	bool operator==(Iterator other){ return this->ptr == other.ptr;}
	bool operator!=(Iterator other){ return this->ptr != other.ptr;}
};

template<typename key_t, typename data_t, typename rank_t>
Iterator<key_t, data_t, rank_t>& Iterator<key_t, data_t, rank_t>::next(){
	Node<key_t, data_t, rank_t>* parent;
  
	if(order >= 0){			// for forward movement. From lowest salary higher id, to highest salry lower id.
		if (ptr != nullptr){
			if (ptr->right != nullptr)
			{
				//have right son
				ptr = ptr->right;

				//take the leftmost son of the right son
				while (ptr->left != nullptr) {
				ptr = ptr->left;
				}
			}
			else
			{
				//look for the first parent from a left son
				parent = ptr->father;
				while (parent != nullptr && ptr == parent->right)
				{
					ptr = parent;
					parent = parent->father;
				}

				//if we were in the last node of the in-order arrange, ptr gets null ptr thats equal to end()
				ptr = parent;
			}
		}
	}
	else{				// for backwards movement. From highest salary lower id, to lower salry higher id.
		if (ptr != nullptr){
			if (ptr->left != nullptr)
			{
				//have left son
				ptr = ptr->left;

				//take the rightmost son of the left son
				while (ptr->right != nullptr) {
				ptr = ptr->right;
				}
			}
			else
			{
				//look for the first parent from a right son
				parent = ptr->father;
				while (parent != nullptr && ptr == parent->left)
				{
					ptr = parent;
					parent = parent->father;
				}

				//if we were in the first node of the in-order arrange, ptr gets null ptr thats equal to end()
				ptr = parent;
			}
		}
	}
	
	return *this;

}

#endif //ITERATOR_H
