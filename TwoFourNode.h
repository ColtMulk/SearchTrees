//
// Created by ColtM on 11/12/2019.
//

#ifndef SEARCHTREES_TWOFOURNODE_CPP
#define SEARCHTREES_TWOFOURNODE_CPP

#include <utility>
#include <iostream>

using namespace std;

template<class key, class value, class compare>
struct TwoFourNode;

template<class key, class value, class compare>
struct node;



template<class key, class value, class compare>
struct TwoFourNode {
    unsigned int length;
    compare lessThan;
    node<key, value, compare> *root;
    TwoFourNode<key, value, compare> *parent;
    TwoFourNode<key, value, compare> *leftSib;
    TwoFourNode<key, value, compare> *rightSib;

    TwoFourNode(key a, value b) {
        root = new node<key, value, compare>(a, b);
        length = 1;
        parent = nullptr;
        leftSib = nullptr;
        rightSib = nullptr;
    }

    TwoFourNode(node<key, value, compare> *linkedList, unsigned int a) {
        root = linkedList;
        parent = nullptr;
        leftSib = nullptr;
        rightSib = nullptr;
        length = a;
    }


    TwoFourNode<key, value, compare> *findInsert(key k) {
        //finds TwoFourNode to insert into
        node<key, value, compare> *curr = root;
        while (curr != nullptr) {
            key current = curr->keyValue->first;
            if (current == k) {
                return nullptr; //nullptr is error (means duplicate)
            } else if (lessThan(k, current)) {
                if (curr->left == nullptr) {
                    return this;
                }
                return curr->left->findInsert(k);
            } else if (curr->rightSibling == nullptr) {
                if (curr->right == nullptr) {
                    return this;
                }
                return curr->right->findInsert(k);
            } else {
                curr = curr->rightSibling;
            }

        }
        return nullptr;
    }

    TwoFourNode<key, value, compare> *findRemove(key k) {
        //finds remove node to erase from
        node<key, value, compare> *curr = root;
        while (curr != nullptr) {
            key current = curr->keyValue->first;
            if (current == k) {
                return this;
            } else if (lessThan(k, current)) {
                if (curr->left == nullptr) {
                    cout << "Error erase key not found" << endl;
                    exit(1);
                }
                return curr->left->findRemove(k);
            } else if (curr->rightSibling == nullptr) {
                if (curr->right == nullptr) {
                    cout << "Error erase key not found" << endl;
                    exit(1);
                }
                return curr->right->findRemove(k);
            } else {
                curr = curr->rightSibling;
            }
        }
        cout << "Error erase key not found" << endl;
        exit(1);
    }

    node<key, value, compare> *search(key k) {
        //finds key k in tree, recursively search appropiate child
        node<key, value, compare> *curr = root;
        while (curr != nullptr) {
            key current = curr->keyValue->first;
            if (current == k) {
                return curr;
            } else if (lessThan(k, current)) {
                if (curr->left == nullptr) {
                    return nullptr;
                }
                return curr->left->search(k);
            } else if (curr->rightSibling == nullptr) {
                if (curr->right == nullptr) {
                    return nullptr;
                }
                return curr->right->search(k);
            } else {
                curr = curr->rightSibling;
            }

        }
        return nullptr;
    }

    node<key, value, compare> *insert(key k, value v) {
        //inserts in this TwoFourNode (doesn't move down tree at all)
        node<key, value, compare> *curr = root;
        node<key, value, compare> *add = new node<key, value, compare>(k, v);
        length++;
        while (curr != nullptr) {
            if (lessThan(k, curr->keyValue->first)) {
                if (curr->leftSibling != nullptr) {
                    add->leftSibling = curr->leftSibling;
                    curr->leftSibling->rightSibling = add;
                } else {
                    root = add;
                }

                add->rightSibling = curr;
                curr->leftSibling = add;
                break;
            } else if (curr->rightSibling == nullptr) {
                //inserts on right
                curr->rightSibling = add;
                add->leftSibling = curr;
                break;
            }
            curr = curr->rightSibling;
        }
        return add;

    }

    TwoFourNode<key, value, compare> *split() {
        //splits this node
        if (length != 4) {
            std::cout << "Error invalid length to split";
            exit(1);
        }
        length--;
        node<key, value, compare> *splitNode = root->rightSibling->rightSibling;

        if (parent == nullptr) {
            //root case
            TwoFourNode<key, value, compare> *right = new TwoFourNode<key, value, compare>(
                    root->rightSibling->rightSibling->rightSibling, 1);
            length = 2;
            TwoFourNode<key, value, compare> *newRoot = new TwoFourNode<key, value, compare>(splitNode, 1);
            splitNode->rightSibling = nullptr;
            splitNode->leftSibling = nullptr;
            right->root->leftSibling = nullptr;
            root->rightSibling->rightSibling = nullptr;

            splitNode->right = right;
            splitNode->left = this;
            right->parent = newRoot;
            this->parent = newRoot;

            //set children
            if(this->root->rightSibling->right != nullptr){
                this->root->rightSibling->right->rightSib = nullptr;
            }
            if(right->root->left != nullptr){
                right->root->left->leftSib = nullptr;
                right->root->left->parent = right;
                right->root->right->parent = right;
            }

            //set siblings
            right->rightSib = this->rightSib;
            if(this->rightSib!=nullptr){
                this->rightSib->leftSib = right;
            }
            this->rightSib = right;
            right->leftSib = this;



            return newRoot;
        } else {
            //othercase
            TwoFourNode<key, value, compare> *right = new TwoFourNode<key, value, compare>(
                    root->rightSibling->rightSibling->rightSibling, 1);
            length = 2;
            node<key, value, compare> *newAdd = parent->insert(splitNode->keyValue->first, splitNode->keyValue->second);
            delete splitNode;
            right->root->leftSibling = nullptr;
            root->rightSibling->rightSibling = nullptr;

            newAdd->right = right;
            newAdd->left = this;
            if (newAdd->leftSibling != nullptr) {
                newAdd->leftSibling->right = this;
            }
            if (newAdd->rightSibling != nullptr) {
                newAdd->rightSibling->left = right;
            }

            //set children
            if(this->root->rightSibling->right != nullptr){
                this->root->rightSibling->right->rightSib = nullptr;
            }
            if(right->root->left != nullptr){
                right->root->left->leftSib = nullptr;
                right->root->left->parent = right;
                right->root->right->parent = right;
            }

            //set sibling pointers
            right->rightSib = this->rightSib;
            if(this->rightSib != nullptr){
                this->rightSib->leftSib = right;
            }
            this->rightSib = right;
            right->leftSib = this;



            right->parent = parent;
            if (parent->length == 4) {
                return parent->split();
            }
        }

        return nullptr;

    }


};

template<class key, class value, class compare>
struct node {
    std::pair<key, value> *keyValue;
    TwoFourNode<key, value, compare> *left;
    TwoFourNode<key, value, compare> *right;
    node<key, value, compare> *rightSibling;
    node<key, value, compare> *leftSibling;

    node(key a, value b) {
        keyValue = new std::pair<key, value>(a, b);
        left = nullptr;
        right = nullptr;
        leftSibling = nullptr;
        rightSibling = nullptr;
    };
};

#endif