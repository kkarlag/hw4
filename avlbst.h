#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); 
    virtual void remove(const Key& key); 
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    // virtual AVLNode<Key,Value>* insertInternal( AVLNode<Key, Value>* node, const Key& key);
    static int height(AVLNode<Key,Value>* node);
    AVLNode<Key,Value> *rightRotate(AVLNode<Key,Value> *node);
    AVLNode<Key,Value> *leftRotate(AVLNode<Key,Value> *node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    Node<Key, Value>* temp = this->search(this->root_, new_item.first);
    if(temp)
        temp->setValue(new_item.second);
    else
        this->root_ = this->insertInternal(this->root_, new_item);
}


template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key,Value>* node){
    if(node == NULL)
        return 0;
    return node->getBalance();
}

template<class Key, class Value>
AVLNode<Key,Value> * AVLTree<Key, Value>::rightRotate(AVLNode<Key,Value> *node)
{
    AVLNode<Key,Value> *x = node->getLeft();
    AVLNode<Key,Value> *T2 = x->getRight();
 
    x->setRight(node);
    node->setLeft(T2);
 
    node->setBalance(((height(node->getLeft()) > height(node->getRight())) ?  height(node->getLeft()) : height(node->getRight())) + 1);
    x->setBalance(((height(x->getLeft()) > height(x->getRight())) ?  height(x->getLeft()) : height(x->getRight())) + 1);
 
    return x;
}
template<class Key, class Value>
AVLNode<Key,Value> *AVLTree<Key, Value>::leftRotate(AVLNode<Key,Value> *node)
{
    AVLNode<Key,Value> *y  = node->getRight();
    AVLNode<Key,Value> *T2 = y->getLeft();
 
    y->setLeft(node);
    node->setRight(T2);
 
    node->setBalance(((height(node->getLeft()) > height(node->getRight())) ?   height(node->getLeft()) :  height(node->getRight())) + 1);
    y->setBalance(((height(y->getLeft()) > height(y->getRight())) ?   height(y->getLeft()) :  height(y->getRight())) + 1);
 
    return y;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    Node<Key,Value>* temp = this->search(this->root_, key);
    if(temp){
        if(temp->getLeft() && temp->getRight())
            nodeSwap((AVLNode<Key,Value>*)this->predecessor(temp), (AVLNode<Key,Value>*)temp);
        else if(temp->getLeft())
            nodeSwap((AVLNode<Key,Value>*)temp->getLeft(), (AVLNode<Key,Value>*)temp);
        else if(temp->getRight())
            nodeSwap((AVLNode<Key,Value>*)temp->getRight(), (AVLNode<Key,Value>*)temp);

        free(temp);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

