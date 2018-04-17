#ifndef TREE_H
#define TREE_H
#include <algorithm>
#include <iostream>
#include "ConstForwardIterator.h"

template<class Key>
struct Node{
    Node<Key>* childs [4];
    Node<Key>* parent;
    Key keys[3];
    int length = 0;

    Key max;

    Node<Key>* right = nullptr;
    Node<Key>* left = nullptr;

	Node(Node<Key>* p):parent(p){
		std::fill(childs, childs+4, nullptr);
	}
    Node(Node<Key>* p, Key value):parent(p), max(value){
        keys[0] = value;
        std::fill(childs, childs+4, nullptr);
    }

    ~Node(){
        for(int i = 0;i<length;++i)
            delete childs[i];
    }
};

template<class Key, class Compare>
class Tree;

template <class Key, class Compare>
std::ostream& operator<<(std::ostream& os, const Tree<Key, Compare>& tree){
    for(auto it = tree.begin(); it!=tree.end();++it)
        os<<*it<<" ";

    os<<'\n';

    return os;
}


template<class Key, class Compare = std::less<Key>>
class Tree{
public:
    Tree(){
        comp = Compare();
    }
    Tree(std::initializer_list<Key>);
    Tree(const Tree&);
    Tree(Tree&&);
    Tree& operator=(const Tree&);
    Tree& operator=(Tree&&);

    Tree operator+(const Tree&) const;//XOR
    Tree operator|(const Tree&) const;//Union
    Tree operator&(const Tree&) const;//Intersection
    Tree operator-(const Tree&) const;//Difference

    void insert(Key);
    void erase(Key);
    void erase(Node<Key>*);
    bool exist(Key) const;
    void clear();

    void swap(Tree&);

    ConstForwardIterator<Key> begin() const;
    ConstForwardIterator<Key> end() const;

    ~Tree();

private:
    Compare comp;
    Node<Key>* root = nullptr;

    Node<Key>* find(Key) const;
    Node<Key>* next(Node<Key>*) const;
    Node<Key>* prev(Node<Key>*) const;
    void updateKeys(Node<Key>*);
    void splitParent(Node<Key>*);
    void sortChilds(Node<Key>*);


    friend std::ostream& operator<< <Key, Compare>(std::ostream&, const Tree&);
};


template <class Key, class Compare>
Tree<Key, Compare>::Tree(std::initializer_list<Key> arr) {
    this->comp = Compare();
    for(auto val : arr)
        this->insert(val);
}


template<class Key, class Compare>
void Tree<Key, Compare>::swap(Tree<Key, Compare>& other){
    std::swap(this->root, other.root);
}


template<class Key, class Compare>
Tree<Key, Compare>::Tree(const Tree<Key, Compare>& other){
    this->comp = other.comp;
    for(auto it = other.begin(); it!=other.end(); ++it)
        this->insert(*it);
}


template<class Key, class Compare>
Tree<Key, Compare>::Tree(Tree<Key, Compare>&& other){
    this->comp = other.comp;
    swap(other);
}


template<class Key, class Compare>
Tree<Key, Compare>& Tree<Key, Compare>::operator=(const Tree<Key, Compare>& other){
    this->comp = other.comp;
    Tree<Key, Compare> tmp(other);

    this->swap(tmp);

    return *this;
}


template<class Key, class Compare>
Tree<Key, Compare>& Tree<Key, Compare>::operator=(Tree<Key, Compare> && other){
    this->comp = other.comp;
    swap(other);

    return *this;
}


template<class Key, class Compare>
Node<Key>* Tree<Key, Compare>::find(Key value) const{
    Node<Key>* cur = root;
    while(cur!=nullptr&&cur->length!=0){
        if(cur->length == 2)
            cur = (comp(value, cur->keys[0])||(!comp(value,cur->keys[0])&&!comp(cur->keys[0],value)))
                  ? cur->childs[0] : cur->childs[1];
        else
            cur = (comp(value, cur->keys[0])||(!comp(value,cur->keys[0])&&!comp(cur->keys[0],value)))
                  ? cur->childs[0]:
                  ((comp(value, cur->keys[1])||(!comp(value,cur->keys[1])&&!comp(cur->keys[1],value)))
                   ?cur->childs[1]:cur->childs[2]);
    }

    return cur;
}


template<class Key, class Compare>
void Tree<Key, Compare>::sortChilds(Node<Key>* node){
    for(int i = node->length-1;i>=0;i--)
        for(int j = 0;j<i;j++)
            if(!comp(node->childs[j]->max,
                     node->childs[j+1]->max))
            {
                Node<Key>* temp = node->childs[j+1];
                node->childs[j+1] = node->childs[j];
                node->childs[j] = temp;
            }
}


template<class Key, class Compare>
void Tree<Key, Compare>::updateKeys(Node<Key>* node){
    Node<Key>* cur = node->parent;
    while(cur!=nullptr){
        for(int i = 0;i< cur->length-1;i++)
            cur->keys[i] = cur->childs[i]->max;


        cur->max = cur->childs[cur->length-1]->max;
        cur = cur->parent;
    }
}


template<class Key, class Compare>
void Tree<Key, Compare>::splitParent(Node<Key>* node){
    if(node->length > 3){
        Node<Key>* new_node = new Node<Key>(node->parent);
        node->childs[2]->parent = new_node;
        node->childs[3]->parent = new_node;
        new_node->childs[0] = node->childs[2];
        new_node->childs[1] = node->childs[3];
        new_node->length = 2;
        new_node->keys[0] = new_node->childs[0]->max;
        new_node->max = new_node->childs[1]->max;

        node->childs[2] = nullptr;
        node->childs[3] = nullptr;
        node->keys[0] = node->childs[0]->max;
        node->max = node->childs[1]->max;
        node->length = 2;

        if(new_node->parent!=nullptr){
            Node<Key>* parent = new_node->parent;
            parent->childs[parent->length++] = new_node;
            this->sortChilds(parent);
            parent->max = parent->childs[parent->length-1]->max;
            this->splitParent(new_node->parent);
        }else{
            root = new Node<Key>(nullptr);

            node->parent = root;
            new_node->parent = root;
            root->length = 2;
            root->childs[0] = node;
            root->childs[1] = new_node;
            root->keys[0] = root->childs[0]->max;
            root->max = root->childs[1]->max;
        }
    }
}



template<class Key, class Compare>
void Tree<Key, Compare>::insert(Key value){
    Node<Key>* cur = this->find(value);

    if(cur == nullptr)
        root = new Node<Key>(nullptr, value);

    else if(cur->parent == nullptr){
        if(comp(cur->keys[0],value)||comp(value, cur->keys[0])){
            Node<Key>* new_node_left = new Node<Key>(cur, (comp(cur->keys[0],value))?cur->keys[0]:value);
            Node<Key>* new_node_right = new Node<Key>(cur, (!comp(cur->keys[0],value))?cur->keys[0]:value);

            new_node_left->right = new_node_right;
            new_node_right->left = new_node_left;

            cur->childs[0] = new_node_left;
            cur->childs[1] = new_node_right;
            cur->keys[0] = new_node_left->max;

            cur->length = 2;
            cur->max = cur->childs[1]->max;
        }
    }
    else{
        if(comp(cur->keys[0], value) || comp(value, cur->keys[0])){
            Node<Key>* new_node = new Node<Key>(cur->parent, value);

            cur->parent->childs[cur->parent->length++] = new_node;

            this->sortChilds(new_node->parent);
            this->updateKeys(new_node);
            this->splitParent(new_node->parent);
            this->updateKeys(new_node);

            Node<Key>* next_ = this->next(new_node);
            Node<Key>* prev_ = this->prev(new_node);

            
            if(next_!=nullptr){
                new_node->right = next_;
                next_->left = new_node;
            }

            if(prev_!=nullptr){
                new_node->left = prev_;
                prev_->right = new_node;
            }
        }
    }
}


template<class Key, class Compare>
void Tree<Key, Compare>::erase(Node<Key>* node){
    Node<Key> *parent = node->parent;

    if(node->length<2){
        Node<Key>* right_ = this->next(node);
        Node<Key>* left_ = this->prev(node);

        if(right_!=nullptr)
            right_->left = left_;

        if(left_!=nullptr)
            left_->right = right_;
    }

    if(parent == nullptr){
        delete node;
        root = nullptr;
    }
    else if(parent->length == 3){
        if(node->parent->childs[0]==node){
            parent->childs[0] = parent->childs[1];
            parent->childs[1] = parent->childs[2];
        }
        else if(node->parent->childs[1] == node){
            parent->childs[1] = parent->childs[2];
        }

        delete node;
        node = nullptr;

        parent->length--;
        parent->childs[2] = nullptr;
        parent->max = parent->childs[1]->max;
        this->updateKeys(parent->childs[0]);

    }else{
        if(parent->parent == nullptr){
			Node<Key>* prev_root = root;
            root = (parent->childs[0]==node)?parent->childs[1]:parent->childs[0];
            root->parent = nullptr;
			prev_root->childs[0] = nullptr;
			prev_root->childs[1] = nullptr;
			delete prev_root;
            delete node;
        }else{
            Node<Key>* nb = (parent->childs[0]==node)?parent->childs[1]:parent->childs[0];
            Node<Key>* gnb;
            if(parent->parent->childs[0]==parent)
                gnb = parent->parent->childs[1];
            else if(parent->parent->childs[1] == parent)
                gnb = parent->parent->childs[0];
            else
                gnb = parent->parent->childs[1];

            parent->childs[0] = nullptr;
            parent->childs[1] = nullptr;

            nb->parent = gnb;
            gnb->childs[gnb->length++] = nb;

            this->sortChilds(gnb);
            this->updateKeys(nb);
            this->splitParent(gnb);

			delete node;
			node = nullptr;

            this->erase(parent);

            this->updateKeys(nb);
        }

    }
}


template<class Key, class Compare>
void Tree<Key, Compare>::erase(Key value){
    Node<Key>* node = this->find(value);
    if(node!=nullptr&&(!comp(node->keys[0],value)&&!comp(value, node->keys[0])))
        this->erase(node);

}


template<class Key, class Compare>
Node<Key>* Tree<Key, Compare>::next(Node<Key>* node) const{
    Node<Key>* parent = node->parent;
    while(parent!=nullptr){
        for(int i = 0;i<parent->length-1;i++)
            if(parent->childs[i]==node){
                node = parent->childs[i+1];

                while(node->length>1)
                    node = node->childs[0];

                return node;
            }


        node = parent;
        parent = node->parent;
    }

    return nullptr;
}


template<class Key, class Compare>
Node<Key>* Tree<Key, Compare>::prev(Node<Key>* node) const{
    Node<Key>* parent = node->parent;
    while(parent!=nullptr){
        for(int i = parent->length-1;i>0;i--)
            if(parent->childs[i]==node){
                node = parent->childs[i-1];

                while(node->length>1)
                    node = node->childs[node->length - 1];

                return node;
            }


        node = parent;
        parent = node->parent;
    }

    return nullptr;
}


template<class Key, class Compare>
bool Tree<Key, Compare>::exist(Key value) const{
    Node<Key>* node = this->find(value);

    return (node!=nullptr&&(!comp(node->keys[0],value)&&!comp(value, node->keys[0])));
}


template<class Key, class Compare>
void Tree<Key, Compare>::clear(){
    delete root;
    root = nullptr;
}


template<class Key, class Compare>
Tree<Key, Compare>::~Tree(){
    this->clear();
}


template <class Key, class Compare>
Tree<Key, Compare> Tree<Key, Compare>::operator&(const Tree<Key, Compare>& other) const{
    auto it1 = this->begin();
    auto it2 = other.begin();

    Tree<Key, Compare> result;

    while(it1!=this->end()&&it2!=other.end()){
        if(!comp(*it1,*it2)&&!comp(*it2, *it1)) {
            result.insert(*it1);
            ++it1;
            ++it2;
        }
        else if(comp(*it1, *it2))
            ++it1;
        else
            ++it2;
    }

    return result;
}


template <class Key, class Compare>
Tree<Key, Compare> Tree<Key, Compare>::operator+(const Tree<Key, Compare>& other) const{
    auto it1 = this->begin();
    auto it2 = other.begin();

    Tree<Key, Compare> result;

    while(it1!=this->end()&&it2!=other.end()){
        if(!comp(*it1,*it2)&&!comp(*it2, *it1)) {
            ++it1;
            ++it2;
        }
        else if(comp(*it1, *it2)) {
            result.insert(*it1);
            ++it1;
        }
        else {
            result.insert(*it2);
            ++it2;
        }
    }

    while(it1!=this->end()){
        result.insert(*it1);
        ++it1;
    }

    while(it2!=other.end()){
        result.insert(*it2);
        ++it2;
    }

    return result;
}


template <class Key, class Compare>
Tree<Key, Compare> Tree<Key, Compare>::operator-(const Tree<Key, Compare>& other) const{
    auto it1 = this->begin();
    auto it2 = other.begin();

    Tree<Key, Compare> result;

    while(it1!=this->end()&&it2!=other.end()){
        if(!comp(*it1,*it2)&&!comp(*it2, *it1)) {
            ++it1;
            ++it2;
        }
        else if(comp(*it1, *it2)) {
            result.insert(*it1);
            ++it1;
        }
        else {
            ++it2;
        }
    }

    while(it1!=this->end()){
        result.insert(*it1);
        ++it1;
    }

    return result;
}


template <class Key, class Compare>
Tree<Key, Compare> Tree<Key, Compare>::operator|(const Tree<Key, Compare>& other) const{
    auto it1 = this->begin();
    auto it2 = other.begin();

    Tree<Key, Compare> result;

    while(it1!=this->end()&&it2!=other.end()){
        if(!comp(*it1,*it2)&&!comp(*it2, *it1)) {
            result.insert(*it1);
            ++it1;
            ++it2;
        }
        else if(comp(*it1, *it2)) {
            result.insert(*it1);
            ++it1;
        }
        else {
            result.insert(*it2);
            ++it2;
        }
    }

    while(it1!=this->end()){
        result.insert(*it1);
        ++it1;
    }

    while(it2!=other.end()){
        result.insert(*it2);
        ++it2;
    }

    return result;
}


template<class Key, class Compare>
ConstForwardIterator<Key> Tree<Key, Compare>::begin() const{
    const Node<Key>* cur = root;
    while(cur!=nullptr&&cur->length > 1)
        cur = cur->childs[0];

    return ConstForwardIterator<Key>(cur);
}


template<class Key, class Compare>
ConstForwardIterator<Key> Tree<Key, Compare>::end() const{
    return ConstForwardIterator<Key>(nullptr);
}

#endif
