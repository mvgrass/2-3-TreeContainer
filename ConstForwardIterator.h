#ifndef CONST_FORWARD_ITERATOR_H
#define CONST_FORWARD_ITERATOR_H

#include <iterator>

template<class Key, class Compare>
class Tree;

template<class Key>
struct Node;

template<class Key>
class ConstForwardIterator: public std::iterator<std::forward_iterator_tag, Key>{
public:
    ConstForwardIterator(const Node<Key>* node):p(node){}
    
	ConstForwardIterator& operator++(){
        if(p!=nullptr)
            p = p->right;

        return *this;
    }

    ConstForwardIterator operator++(int v){
        ConstForwardIterator temp = *this;

		this->operator++();

        return temp;
    }


    bool operator!=(const ConstForwardIterator& other) const{
        return this->p != other.p;
    }

	bool operator==(const ConstForwardIterator& other) const {
		return !(this->operator!=(other));
	}

    Key operator*() const{
        return p->keys[0];
    }

	const Key* operator->() const{
		return p->keys;
	}
private:
    const Node<Key>* p;

};

#endif
